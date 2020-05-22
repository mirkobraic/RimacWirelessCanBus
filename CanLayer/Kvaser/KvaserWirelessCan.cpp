#include "KvaserWirelessCan.h"

KvaserWirelessCan::KvaserWirelessCan()
{
}

KvaserWirelessCan::~KvaserWirelessCan()
{
    disconnectFromDevice();
}

void KvaserWirelessCan::connectToDevice(QString deviceIpAddress, BaudRate baudRate)
{
    if (isConnected) {
        return;
    }

    this->deviceIpAddress = deviceIpAddress;
    this->port = "8080";
    kvNetService.setupDevice(this->deviceIpAddress, this->port);

    kvNetService.initializeLibrary([=](KvaserNetworkService::Status stat, QString sessionId) {
        if (checkStatus("initializeLibrary", stat) == false) {
            return;
        }
        this->sessionId = sessionId;

        kvNetService.openChannel(this->sessionId, [=](KvaserNetworkService::Status stat, int handle) {
            if (checkStatus("openChannel", stat) == false) {
                return;
            }
            this->txHandle = handle;

            kvNetService.setBusParams(this->sessionId, this->txHandle, getBaudRate(baudRate), [=](KvaserNetworkService::Status stat) {
                if (checkStatus("setBusParams", stat) == false) {
                    return;
                }

                kvNetService.canBusOn(this->sessionId, this->txHandle, [=](KvaserNetworkService::Status stat) {
                    if (checkStatus("canBusOn", stat)) {
                        isConnected = true;
                        shouldListen = true;
                        QtConcurrent::run(this, &KvaserWirelessCan::startRxThread);
                    }
                });
            });
        });
    });

    /*****/
/***

    // TODO: try to improve this
    baseUrl.setUrl("http://" + deviceIpAddress + ":8080");

    initializeLibrary([=] {
        openChannel(&txHandle, [=] {
            setBusParams(txHandle, getBaudRate(baudRate), [=] {
                canBusOn(txHandle, [=] {
                    isConnected = true;
                    shouldListen = true;
//                    QtConcurrent::run(this, &KvaserWirelessCan::startRxThread);
                });
            });
        });
    });
***/
    /***
    if (isConnected) {
        return;
    }

    qDebug() << "--CanLayer: number of channels: " << getChannelCount();

    //TODO: implement channelName
    channelNumber = 0;

    txHandle = canOpenChannel(channelNumber, canOPEN_ACCEPT_VIRTUAL);
    if (txHandle < 0){
        txStatus = (canStatus)txHandle;
        checkStatus("canOpenChannel", txStatus);
    }

    txStatus = canSetBusParams(txHandle, getBaudRate(baudRate), 0, 0, 0, 0, 0);
    checkStatus("canSetBusParams", txStatus);

    txStatus = canBusOn(txHandle);
    checkStatus("canBusOn", txStatus);

    isConnected = true;
    shouldListen = true;

    // start rx thread
    QtConcurrent::run(this, &KvaserWirelessCan::startListening);

    ***/
}

void KvaserWirelessCan::disconnectFromDevice()
{
    if (isConnected == false) {
        return;

//        txStatus = canBusOff(txHandle);
//        checkStatus("canBusOff", txStatus);

//        txStatus = canClose(txHandle);
//        checkStatus("canClose", txStatus);
//        txHandle = canINVALID_HANDLE;
    }

    isConnected = false;
    shouldListen = false;

    kvNetService.canBusOff(sessionId, txHandle, [=](KvaserNetworkService::Status stat) {
        checkStatus("canBusOff", stat);

        kvNetService.closeChannel(sessionId, txHandle, [=](KvaserNetworkService::Status stat) {
            checkStatus("closeChannel", stat);

            kvNetService.unloadLibrary(sessionId, [=](KvaserNetworkService::Status stat) {
                checkStatus("closeChannel", stat);
            });
        });
    });


//    canBusOff(txHandle, [=] {
//        closeChannel(txHandle, [=] {
//            unloadLibrary({});
//        });
//    });


}

void KvaserWirelessCan::sendCanMessage(isotp::can_layer_message &message)
{
    /***
//    char *data = reinterpret_cast<char*>(message.data.data());
    QString data;
    for (size_t i = 0; i < message.data.size(); i++) {
        if (i != 0) {
            data += ",";
        }
        data += QString::number(message.data[i]);
    }
    uint flag = message.id > maxStdCanId ? canMSG_EXT : canMSG_STD;

//    canFlushTransmitQueue(txHandle);
//    txStatus = canWrite(txHandle, message.id, data, message.data.size(), flag);

    qDebug() << "CanLayer: writing ID =" << message.id << " data =" << message.data;
    baseUrl.setPath("/" + sessionId + "/canWrite");
    QUrlQuery query;
    query.addQueryItem("hnd", QString::number(txHandle));
    query.addQueryItem("id", QString::number(message.id));
    query.addQueryItem("flag", QString::number(flag));
    query.addQueryItem("msg", data);
    query.addQueryItem("dlc", QString::number(message.data.size()));

    baseUrl.setQuery(query.query());
    qDebug() << baseUrl;
//    baseUrl.setQuery("hnd=" + handle + "&id=1473&flag=0&msg=255,255,255,255,255,255,255,255&dlc=8");
    QNetworkReply *reply = networkManager.get(QNetworkRequest(baseUrl));
    auto completion = [=]() {
        if (reply->error()) {
            qDebug() << reply->errorString();
            return;
        }
        QJsonDocument jsdoc = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsobj = jsdoc.object();
        KvaserStatus stat = (KvaserStatus)jsobj["stat"].toInt();
        checkStatus("canWrite", stat);
    };
    QObject::connect(reply, &QNetworkReply::finished, this, completion);
    ***/

    QString data;
    for (size_t i = 0; i < message.data.size(); i++) {
        if (i != 0) {
            data += ",";
        }
        data += QString::number(message.data[i]);
    }
    uint flag = message.id > maxStdCanId ? canMSG_EXT : canMSG_STD;

    kvNetService.canWrite(sessionId, txHandle, message.id, flag, data, message.data.size(), [=](KvaserNetworkService::Status stat) {
        checkStatus("canWrite", stat);
    });
}

void KvaserWirelessCan::startRxThread()
{
    // different handle is reqired for running on a different thread
//    int rxHandle;

//    openChannel(&rxHandle, [=] {
//        canBusOn(rxHandle, [=] {
//            while(shouldListen) {
//                canRead(rxHandle);
//            }
//            canBusOff(rxHandle, [=] {
//                closeChannel(rxHandle,{});
//            });
//        });
//    });

    rxKvNetService = new KvaserNetworkService();
    rxKvNetService->setupDevice(this->deviceIpAddress, this->port);

    qDebug() << "Rx thread";
    rxKvNetService->openChannel(sessionId, [=](KvaserNetworkService::Status stat, int rxHandle) {
            qDebug() << "openChannel";
        if (checkStatus("openChannel", stat) == false) {
            return;
        }

        rxKvNetService->canBusOn(sessionId, rxHandle, [=](KvaserNetworkService::Status stat) {
            qDebug() << "canBusOn";
            if (checkStatus("canBusOn", stat) == false) {
                return;
            }

            while(shouldListen) {
                rxKvNetService->canRead(sessionId, rxHandle, [=](KvaserNetworkService::Status stat, uint32_t id, std::vector<uint8_t> data) {
                    qDebug() << "canRead";
                    if (checkStatus("canRead", stat) == false) {
                        return;
                    }

                    isotp::can_layer_message msg;
                    msg.id = id;
                    msg.data = data;
                    messageRecievedUdsCallback(std::make_unique<isotp::can_layer_message>(msg));
                    messageRecievedDirectCallback(id, data);
                });
            }
        });
    });

    /***
//    canHandle rxHandle = canINVALID_HANDLE;
    canStatus rxStatus = canStatus(0);

    rxHandle = canOpenChannel(channelNumber, canOPEN_ACCEPT_VIRTUAL);
    if (rxHandle < 0){
        rxStatus = (canStatus)rxHandle;
//        checkStatus("canOpenChannel", rxStatus);
    }

    // TODO: implement this for final version, commented out because it interfers with virtual can
    // stops messages sent from another thread from being recieved in this thread
//    unsigned char buf = 0x00;
//    rxStatus = canIoCtl(rxHandle, canIOCTL_SET_LOCAL_TXECHO, &buf, sizeof(buf));
//    checkStatus("canIoCtl", rxStatus);

//    rxStatus = canBusOn(rxHandle);
//    checkStatus("canBusOn", rxStatus);

    long id;
    unsigned int dlc, flags;
    char data[8];
    unsigned long timestamp;

    while (shouldListen) {
        rxStatus = canReadWait(rxHandle, &id, data, &dlc, &flags, &timestamp, 100);
        if (rxStatus == canERR_NOMSG){
            // QUESTION: should we add sleep?
            continue;
        }

        if (rxStatus == canOK) {
            if (flags & canMSG_ERROR_FRAME){
                qDebug() << "***ERROR FRAME RECEIVED***";
            } else {
                isotp::can_layer_message msg;
                msg.id = id;
                unsigned int arraySize = sizeof(data) / sizeof(char);
                // assure that dlc is not greater than array size
                if (dlc > arraySize) {
                    dlc = arraySize;
                }
                msg.data = std::vector<uint8_t>(data, data + dlc);
                messageRecievedUdsCallback(std::make_unique<isotp::can_layer_message>(msg));
                messageRecievedDirectCallback(msg.id, msg.data);
                qDebug() << "--CanLayer: recieved ID =" << id << " data =" << msg.data;
            }
        } else {
//            checkStatus("canReadWait", rxStatus);
        }
    }

//    rxStatus = canBusOff(rxHandle);
//    checkStatus("canBusOff", rxStatus);

    rxStatus = canClose(rxHandle);
//    checkStatus("canClose", rxStatus);
    rxHandle = canINVALID_HANDLE;

    ***/
}
/*
void KvaserWirelessCan::canRead(int handle)
{
    // TODO: fix reply delete
    QUrlQuery query;
    query.addQueryItem("hnd", QString::number(handle));

    QUrl url = QUrl::fromUserInput("http://172.20.10.3:8080");
    url.setPath("/" + sessionId + "/canRead");
    url.setQuery(query.query());

    QNetworkAccessManager networkManager;
    QNetworkReply *reply = networkManager.get(QNetworkRequest(url));
    auto completion = [=]() {
        if (reply->error()) {
            qDebug() << reply->errorString();
            return;
        }

        QJsonDocument jsdoc = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsobj = jsdoc.object();
        KvaserStatus stat = (KvaserStatus)jsobj["stat"].toInt();
        if (stat == 0) {
            QJsonArray messages = jsobj["msgs"].toArray();
            foreach (const QJsonValue &messageJson, messages) {
                isotp::can_layer_message msg;
                QJsonObject message = messageJson.toObject();

                msg.id = message["id"].toInt();
                QJsonArray bytes = jsobj["msg"].toArray();
                foreach (const QJsonValue &byte, bytes) {
                    msg.data.push_back(byte.toInt());
                }
                messageRecievedUdsCallback(std::make_unique<isotp::can_layer_message>(msg));
                messageRecievedDirectCallback(msg.id, msg.data);
            }
        } else {
            checkStatus("canRead", stat);
            if (stat == canERR_NOMSG) {
                // TODO: maybe sleep
            }
        }
    };
    QObject::connect(reply, &QNetworkReply::finished, this, completion);


//        rxStatus = canReadWait(rxHandle, &id, data, &dlc, &flags, &timestamp, 100);
//        if (rxStatus == canERR_NOMSG){
//            // QUESTION: should we add sleep?
//            continue;
//        }

//        if (rxStatus == canOK) {
//            if (flags & canMSG_ERROR_FRAME){
//                qDebug() << "***ERROR FRAME RECEIVED***";
//            } else {
//                isotp::can_layer_message msg;
//                msg.id = id;
//                unsigned int arraySize = sizeof(data) / sizeof(char);
//                // assure that dlc is not greater than array size
//                if (dlc > arraySize) {
//                    dlc = arraySize;
//                }
//                msg.data = std::vector<uint8_t>(data, data + dlc);
//                messageRecievedUdsCallback(std::make_unique<isotp::can_layer_message>(msg));
//                messageRecievedDirectCallback(msg.id, msg.data);
//                qDebug() << "--CanLayer: recieved ID =" << id << " data =" << msg.data;
//            }
//        } else {
////            checkStatus("canReadWait", rxStatus);
//        }

}
*/

bool KvaserWirelessCan::checkStatus(QString method, KvaserNetworkService::Status status)
{
    if (status == KvaserNetworkService::Status::canOK) {
        return true;
    }
    QString errorMsg = "CanLayer: " + method + " failed with status: " + QString::number(status);
    qDebug() << errorMsg;
    return false;
}

int KvaserWirelessCan::getBaudRate(BaudRate baudRate)
{
    switch (baudRate){
    case Baud_125:
        return -4;
    case Baud_250:
        return -3;
    case Baud_500:
        return -2;
    case Baud_1000:
        return -1;
    default:
        return -2;
    }
}

/***

void KvaserWirelessCan::checkStatus(QString method, KvaserStatus status)
{
    if (status != canOK) {
        QString errorMsg = "CanLayer: " + method + " failed with status: " + QString::number(status);
        qDebug() << errorMsg;
    }
}

void KvaserWirelessCan::initializeLibrary(std::function<void()> callback)
{
    baseUrl.setPath("/canInitializeLibrary");
    // TODO: used in development
    baseUrl.setQuery("timeout=10");

    QNetworkReply *reply = networkManager.get(QNetworkRequest(baseUrl));
    auto completion = [=]() {
        if (reply->error()) {
            qDebug() << reply->errorString();
            return;
        }

        QJsonDocument jsdoc = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsobj = jsdoc.object();
        KvaserStatus stat = (KvaserStatus)jsobj["stat"].toInt();
        if (stat == 0) {
            sessionId = jsobj["session"].toString();
        } else {
            checkStatus("canInitializeLibrary", stat);
        }

        callback();
    };
    QObject::connect(reply, &QNetworkReply::finished, this, completion);
}

void KvaserWirelessCan::openChannel(int *handle, std::function<void()> callback)
{
    QUrlQuery query;
    query.addQueryItem("channel", "0");
    query.addQueryItem("flags", "8");

    baseUrl.setPath("/" + sessionId + "/canOpenChannel");
    baseUrl.setQuery(query.query());

    QNetworkReply *reply = networkManager.get(QNetworkRequest(baseUrl));
    auto completion = [=]() {
        if (reply->error()) {
            qDebug() << reply->errorString();
            return;
        }

        QJsonDocument jsdoc = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsobj = jsdoc.object();
        KvaserStatus stat = (KvaserStatus)jsobj["stat"].toInt();
        if (stat == 0) {
            *handle = jsobj["hnd"].toInt();
        } else {
            checkStatus("canOpenChannel", stat);
        }

        callback();
    };
    QObject::connect(reply, &QNetworkReply::finished, this, completion);
}

void KvaserWirelessCan::setBusParams(int handle, int baudRate, std::function<void()> callback)
{
    QUrlQuery query;
    query.addQueryItem("hnd", QString::number(handle));
    query.addQueryItem("freq", QString::number(baudRate));

    baseUrl.setPath("/" + sessionId + "/canSetBusParams");
    baseUrl.setQuery(query.query());

    QNetworkReply *reply = networkManager.get(QNetworkRequest(baseUrl));
    auto completion = [=]() {
        if (reply->error()) {
            qDebug() << reply->errorString();
            return;
        }

        QJsonDocument jsdoc = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsobj = jsdoc.object();
        KvaserStatus stat = (KvaserStatus)jsobj["stat"].toInt();
        checkStatus("canSetBusParams", stat);

        callback();
    };
    QObject::connect(reply, &QNetworkReply::finished, this, completion);
}

void KvaserWirelessCan::canBusOn(int handle, std::function<void()> callback)
{
    QUrlQuery query;
    query.addQueryItem("hnd", QString::number(handle));

    baseUrl.setPath("/" + sessionId + "/canBusOn");
    baseUrl.setQuery(query.query());

    QNetworkReply *reply = networkManager.get(QNetworkRequest(baseUrl));
    auto completion = [=]() {
        if (reply->error()) {
            qDebug() << reply->errorString();
            return;
        }
        QJsonDocument jsdoc = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsobj = jsdoc.object();
        KvaserStatus stat = (KvaserStatus)jsobj["stat"].toInt();
        checkStatus("canSetBusParams", stat);

        callback();
    };
    QObject::connect(reply, &QNetworkReply::finished, this, completion);
}

void KvaserWirelessCan::canBusOff(int handle, std::function<void()> callback)
{
    QUrlQuery query;
    query.addQueryItem("hnd", QString::number(handle));

    baseUrl.setPath("/" + sessionId + "/canBusOff");
    baseUrl.setQuery(query.query());

    QNetworkReply *reply = networkManager.get(QNetworkRequest(baseUrl));
    auto completion = [=]() {
        if (reply->error()) {
            qDebug() << reply->errorString();
            return;
        }
        QJsonDocument jsdoc = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsobj = jsdoc.object();
        KvaserStatus stat = (KvaserStatus)jsobj["stat"].toInt();
        checkStatus("canBusOff", stat);

        callback();
    };
    QObject::connect(reply, &QNetworkReply::finished, this, completion);
}

void KvaserWirelessCan::closeChannel(int handle, std::function<void()> callback)
{
    QUrlQuery query;
    query.addQueryItem("hnd", QString::number(handle));

    baseUrl.setPath("/" + sessionId + "/canClose");
    baseUrl.setQuery(query.query());

    QNetworkReply *reply = networkManager.get(QNetworkRequest(baseUrl));
    auto completion = [=]() {
        if (reply->error()) {
            qDebug() << reply->errorString();
            return;
        }
        QJsonDocument jsdoc = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsobj = jsdoc.object();
        KvaserStatus stat = (KvaserStatus)jsobj["stat"].toInt();
        checkStatus("canClose", stat);

        callback();
    };
    QObject::connect(reply, &QNetworkReply::finished, this, completion);
}

void KvaserWirelessCan::unloadLibrary(std::function<void()> callback)
{
    baseUrl.setPath("/" + sessionId + "/canUnloadLibrary");

    QNetworkReply *reply = networkManager.get(QNetworkRequest(baseUrl));
    auto completion = [=]() {
        if (reply->error()) {
            qDebug() << reply->errorString();
            return;
        }
        QJsonDocument jsdoc = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsobj = jsdoc.object();
        KvaserStatus stat = (KvaserStatus)jsobj["stat"].toInt();
        checkStatus("canClose", stat);

//        callback();
    };
    QObject::connect(reply, &QNetworkReply::finished, this, completion);
}
***/
