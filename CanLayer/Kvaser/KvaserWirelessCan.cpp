#include "KvaserWirelessCan.h"

KvaserWirelessCan::KvaserWirelessCan()
{ }

KvaserWirelessCan::~KvaserWirelessCan()
{
    disconnectFromDevice();
    delete rxTimer;
}

void KvaserWirelessCan::connectToDevice(QString deviceIpAddress, BaudRate baudRate)
{
    if (isConnected) {
        return;
    }

    this->deviceIpAddress = deviceIpAddress;
    this->port = "8080";
    kvNetService.setupDevice(this->deviceIpAddress, this->port);

    kvNetService.initializeLibrary([=](ResponseStatus stat, QString sessionId) {
        if (checkStatus("initializeLibrary", stat) == false) {
            return;
        }
        this->sessionId = sessionId;

        kvNetService.openChannel(this->sessionId, channelNumber, canOPEN_EXCLUSIVE, [=](ResponseStatus stat, int handle) {
            if (checkStatus("openChannel", stat) == false) {
                return;
            }
            this->handle = handle;

            kvNetService.setBaudRate(this->sessionId, this->handle, baudRate, [=](ResponseStatus stat) {
                if (checkStatus("setBusParams", stat) == false) {
                    return;
                }

                kvNetService.canBusOn(this->sessionId, this->handle, [=](ResponseStatus stat) {
                    if (checkStatus("canBusOn", stat)) {
                        isConnected = true;
                        rxTimer = new QTimer(this);
                        QObject::connect(rxTimer, &QTimer::timeout, this, &KvaserWirelessCan::readMessage);
                        rxTimer->start(500);
                    }
                });
            });
        });
    });
}

void KvaserWirelessCan::disconnectFromDevice()
{
    if (isConnected == false) {
        return;
    }

    isConnected = false;
    rxTimer->stop();

    kvNetService.canBusOff(sessionId, handle, [=](ResponseStatus stat) {
        checkStatus("canBusOff", stat);

        kvNetService.closeChannel(sessionId, handle, [=](ResponseStatus stat) {
            checkStatus("closeChannel", stat);

            kvNetService.unloadLibrary(sessionId, [=](ResponseStatus stat) {
                checkStatus("closeChannel", stat);
            });
        });
    });
}

void KvaserWirelessCan::sendCanMessage(isotp::can_layer_message &message)
{
    qDebug() << "CanLayer: writing ID =" << message.id << " data =" << message.data;
    QString data;
    for (size_t i = 0; i < message.data.size(); i++) {
        if (i != 0) {
            data += ",";
        }
        data += QString::number(message.data[i]);
    }

    uint flag = message.id > maxStdCanId ? canMSG_EXT : canMSG_STD;

    kvNetService.canWrite(sessionId, handle, message.id, flag, data, message.data.size(), [=](ResponseStatus stat) {
        checkStatus("canWrite", stat);
    });
}

void KvaserWirelessCan::readMessage()
{
    kvNetService.canRead(sessionId, handle, 10, [=](ResponseStatus stat, uint32_t id, uint flag, std::vector<uint8_t> data) {
        if (checkStatus("canRead", stat) == false) {
            return;
        }

        if (flag & canMSG_ERROR_FRAME) {
            qDebug() << "Error frame recieved!";
            return;
        }

        qDebug() << "CanLayer: recieved ID =" << id << " data =" << data;
        isotp::can_layer_message msg;
        msg.id = id;
        msg.data = data;
        messageRecievedUdsCallback(std::make_unique<isotp::can_layer_message>(msg));
        messageRecievedDirectCallback(id, data);
    });
}

bool KvaserWirelessCan::checkStatus(QString method, ResponseStatus status)
{
    if (status == canOK) {
        return true;
    }
    qDebug() << "CanLayer: " + method + " failed with status: " + QString::number(status);
    return false;
}
