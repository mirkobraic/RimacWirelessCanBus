#include "KvaserWirelessCan.h"

KvaserWirelessCan::KvaserWirelessCan()
{ }

KvaserWirelessCan::~KvaserWirelessCan()
{
    disconnectFromDevice();
    delete rxTimer;
}

void KvaserWirelessCan::connectToDevice(QString deviceIpAddress, QString port, BaudRate baudRate)
{
    if (isConnected) {
        return;
    }

    this->deviceIpAddress = deviceIpAddress;
    this->port = port;
    kvNetService.setupDevice(this->deviceIpAddress, this->port);

    kvNetService.initializeLibrary([=](KvaserResponse res, QString sessionId) {
        if (checkStatus("initializeLibrary", res) == false) {
            emit showAlert("Error", res.message);
            return;
        }
        this->sessionId = sessionId;

        kvNetService.openChannel(this->sessionId, channelNumber, canOPEN_EXCLUSIVE, [=](KvaserResponse res, int handle) {
            if (checkStatus("openChannel", res) == false) {
                return;
            }
            this->handle = handle;

            kvNetService.setBaudRate(this->sessionId, this->handle, baudRate, [=](KvaserResponse res) {
                if (checkStatus("setBusParams", res) == false) {
                    return;
                }

                kvNetService.canBusOn(this->sessionId, this->handle, [=](KvaserResponse res) {
                    if (checkStatus("canBusOn", res)) {
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

    kvNetService.canBusOff(sessionId, handle, [=](KvaserResponse res) {
        checkStatus("canBusOff", res);

        kvNetService.closeChannel(sessionId, handle, [=](KvaserResponse res) {
            checkStatus("closeChannel", res);

            kvNetService.unloadLibrary(sessionId, [=](KvaserResponse res) {
                checkStatus("closeChannel", res);
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

    kvNetService.canWrite(sessionId, handle, message.id, flag, data, message.data.size(), [=](KvaserResponse res) {
        checkStatus("canWrite", res);
    });
}

void KvaserWirelessCan::readMessage()
{
    kvNetService.canRead(sessionId, handle, 10, [=](KvaserResponse res, uint32_t id, uint flag, std::vector<uint8_t> data) {
        if (checkStatus("canRead", res) == false) {
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

bool KvaserWirelessCan::checkStatus(QString method, KvaserResponse res)
{
    if (res.status == canOK) {
        return true;
    }
    qDebug() << "CanLayer: " + method + " failed with status: " + QString::number(res.status);
    emit showAlert("Error", res.message);
    return false;
}
