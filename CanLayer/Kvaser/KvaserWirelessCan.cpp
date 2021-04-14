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

    kvNetService.setupDevice(deviceIpAddress, port);

    emit fetchingInProgress(true);
    kvNetService.initializeLibrary([=](KvaserResponse res, QString sessionId) {
        if (checkStatus("initializeLibrary", res) == false) {
            emit fetchingInProgress(false);
            return;
        }
        this->sessionId = sessionId;

        kvNetService.openChannel(this->sessionId, channelNumber, canOPEN_EXCLUSIVE, [=](KvaserResponse res, int handle) {
            if (checkStatus("openChannel", res) == false) {
                emit fetchingInProgress(false);
                return;
            }
            this->handle = handle;

            kvNetService.setBaudRate(this->sessionId, this->handle, baudRate, [=](KvaserResponse res) {
                if (checkStatus("setBusParams", res) == false) {
                    emit fetchingInProgress(false);
                    return;
                }

                kvNetService.canBusOn(this->sessionId, this->handle, [=](KvaserResponse res) {
                    emit fetchingInProgress(false);
                    if (checkStatus("canBusOn", res)) {
                        isConnected = true;
                        toggleConnection(true);
                        rxTimer = new QTimer(this);
                        QObject::connect(rxTimer, &QTimer::timeout, this, &KvaserWirelessCan::readMessage);
                        rxTimer->start(20);
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

    emit fetchingInProgress(true);
    kvNetService.canBusOff(sessionId, handle, [=](KvaserResponse res) {
        if (checkStatus("canBusOff", res) == false) {
            emit fetchingInProgress(false);
            toggleConnection(false);
            return;
        }

        kvNetService.closeChannel(sessionId, handle, [=](KvaserResponse res) {
            if (checkStatus("closeChannel", res) == false) {
                emit fetchingInProgress(false);
                toggleConnection(false);
                return;
            }

            kvNetService.unloadLibrary(sessionId, [=](KvaserResponse res) {
                checkStatus("unloadLibrary", res);
                emit fetchingInProgress(false);
                toggleConnection(false);
            });
        });
    });
}

void KvaserWirelessCan::sendCanMessage(CanMessage &message)
{
    qDebug() << "CanLayer: writing ID =" << message.getId() << " data =" << message.getData();
    QString data;
    for (size_t i = 0; i < message.getData().size(); i++) {
        if (i != 0) {
            data += ",";
        }
        data += QString::number(message.getData()[i]);
    }

    uint flag = message.getId() > maxStdCanId ? canMSG_EXT : canMSG_STD;

    dispatchToMainThread([this, message, flag, data] {
        kvNetService.canWrite(sessionId, handle, message.getId(), flag, data, message.getData().size(), [=](KvaserResponse res) {
            checkStatus("canWrite", res);
        });
    });
}

void KvaserWirelessCan::readMessage()
{
    kvNetService.canRead(sessionId, handle, 10, [=](KvaserResponse res, uint32_t id, uint flag, std::vector<uint8_t> data) {
        Q_UNUSED(res); // we don't want to check status on every read
        if (flag & canMSG_ERROR_FRAME) {
            qDebug() << "Error frame recieved!";
            return;
        }

        qDebug() << "CanLayer: recieved ID =" << id << " data =" << data;
        if (id == 0) { return; }

        CanMessage msg = CanMessage(id, data.size(), data);
        messageRecievedUdsCallback(std::make_unique<CanMessage>(msg));
        emit newDirectCanMessage(id, data);


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
