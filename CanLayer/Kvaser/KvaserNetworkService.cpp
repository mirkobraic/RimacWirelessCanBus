#include "KvaserNetworkService.h"

KvaserNetworkService::KvaserNetworkService(QObject *parent) : QObject(parent)
{ }

void KvaserNetworkService::setupDevice(QString ipAddress, QString port)
{
    baseUrl.setUrl("http://" + ipAddress + ":" + port);
}

int KvaserNetworkService::getKvaserBaudRate(BaudRate baudRate)
{
    switch (baudRate){
    case Baud_125:  return -4;
    case Baud_250:  return -3;
    case Baud_500:  return -2;
    case Baud_1000: return -1;
    default:        return -2;
    }
}

QString KvaserNetworkService::getMessageForStatus(ResponseStatus status)
{
    switch (status) {
    case canOK:                         return "Success";
    case canERR_PARAM:                  return "Error in one or more parameters";
    case canERR_NOMSG:                  return "There were no messages to read";
    case canERR_NOMEM:                  return "Out of memory";
    case canERR_NOCHANNELS:             return "No channels available";
    case canERR_TIMEOUT:                return "Timeout occurred";
    case canERR_INVHANDLE:              return "Handle is invalid";
    case canERR_HARDWARE:               return "A hardware error has occurred";
    case canERR_NOT_IMPLEMENTED:        return "The requested feature or function is not implemented";
    case canERR_DEVICE_FILE:            return "An error has occured when trying to access a file on the device";
    case canERR_SCRIPT_FAIL:            return "A script has failed";
    case canERR_SCRIPT_WRONG_VERSION:   return "The t script version dosen't match the version that the device firmware supports";
    case canERR_INVALID_PASSWORD:       return "Invalid password";
    case canERR_NO_SUCH_FUNCTION:       return "No such function";
    case canERR_NOT_AUTHORIZED:         return "Not authorized";
    case canERR_INVALID_SESSION:        return "Invalid session";
    case NETWORK_ERR:                   return "Network error";
    default:                            return "Unknown error";
    }
}

void KvaserNetworkService::initializeLibrary(std::function<void (KvaserResponse, QString)> callback)
{
    baseUrl.setPath("/canInitializeLibrary");
    // TODO: used in development
    QUrlQuery query;
    query.addQueryItem("timeout", QString::number(10));
    baseUrl.setQuery(query.query());

    QNetworkAccessManager *qnam = new QNetworkAccessManager;
    QNetworkReply *reply = qnam->get(QNetworkRequest(baseUrl));
    auto completion = [=]() {
        KvaserResponse res;
        QString sessionId;

        if (reply->error()) {
            res.status = NETWORK_ERR;
            res.message = reply->errorString();
        } else {
            QJsonDocument jsonDoc = QJsonDocument::fromJson(reply->readAll());
            QJsonObject jsonObj = jsonDoc.object();

            res.status = (ResponseStatus)jsonObj["stat"].toInt();
            res.message = getMessageForStatus(res.status);
            sessionId = jsonObj["session"].toString();
        }

        qnam->deleteLater();
        reply->deleteLater();
        callback(res, sessionId);
    };
    QObject::connect(reply, &QNetworkReply::finished, this, completion);
}

void KvaserNetworkService::openChannel(QString sessionId, int channel, OpenFlags flags, std::function<void (KvaserResponse, int)> callback)
{
    QUrlQuery query;
    query.addQueryItem("channel", QString::number(channel));
    query.addQueryItem("flags", QString::number(flags));

    baseUrl.setPath("/" + sessionId + "/canOpenChannel");
    baseUrl.setQuery(query.query());

    QNetworkAccessManager *qnam = new QNetworkAccessManager;
    QNetworkReply *reply = qnam->get(QNetworkRequest(baseUrl));
    auto completion = [=]() {
        KvaserResponse res;
        int handle = -1;

        if (reply->error()) {
            res.status = NETWORK_ERR;
            res.message = reply->errorString();
        } else {
            QJsonDocument jsonDoc = QJsonDocument::fromJson(reply->readAll());
            QJsonObject jsonObj = jsonDoc.object();

            res.status = (ResponseStatus)jsonObj["stat"].toInt();
            res.message = getMessageForStatus(res.status);
            handle = jsonObj["hnd"].toInt();
        }

        qnam->deleteLater();
        reply->deleteLater();
        callback(res, handle);
    };
    QObject::connect(reply, &QNetworkReply::finished, this, completion);
}

void KvaserNetworkService::setBaudRate(QString sessionId, int handle, BaudRate baudRate, std::function<void (KvaserResponse)> callback)
{
    QUrlQuery query;
    query.addQueryItem("hnd", QString::number(handle));
    query.addQueryItem("freq", QString::number(getKvaserBaudRate(baudRate)));

    baseUrl.setPath("/" + sessionId + "/canSetBusParams");
    baseUrl.setQuery(query.query());

    QNetworkAccessManager *qnam = new QNetworkAccessManager;
    QNetworkReply *reply = qnam->get(QNetworkRequest(baseUrl));
    auto completion = [=]() {
        KvaserResponse res;

        if (reply->error()) {
            res.status = NETWORK_ERR;
            res.message = reply->errorString();
        } else {
            QJsonDocument jsonDoc = QJsonDocument::fromJson(reply->readAll());
            QJsonObject jsonObj = jsonDoc.object();

            res.status = (ResponseStatus)jsonObj["stat"].toInt();
            res.message = getMessageForStatus(res.status);
        }

        qnam->deleteLater();
        reply->deleteLater();
        callback(res);
    };
    QObject::connect(reply, &QNetworkReply::finished, this, completion);
}

void KvaserNetworkService::canBusOn(QString sessionId, int handle, std::function<void (KvaserResponse)> callback)
{
    QUrlQuery query;
    query.addQueryItem("hnd", QString::number(handle));

    baseUrl.setPath("/" + sessionId + "/canBusOn");
    baseUrl.setQuery(query.query());

    QNetworkAccessManager *qnam = new QNetworkAccessManager;
    QNetworkReply *reply = qnam->get(QNetworkRequest(baseUrl));
    auto completion = [=]() {
        KvaserResponse res;

        if (reply->error()) {
            res.status = NETWORK_ERR;
            res.message = reply->errorString();
        } else {
            QJsonDocument jsonDoc = QJsonDocument::fromJson(reply->readAll());
            QJsonObject jsonObj = jsonDoc.object();

            res.status = (ResponseStatus)jsonObj["stat"].toInt();
            res.message = getMessageForStatus(res.status);
        }

        qnam->deleteLater();
        reply->deleteLater();
        callback(res);
    };
    QObject::connect(reply, &QNetworkReply::finished, this, completion);
}

void KvaserNetworkService::canWrite(QString sessionId, int handle, uint32_t id, uint flag, QString data, size_t dlc, std::function<void (KvaserResponse)> callback)
{
    QUrlQuery query;
    query.addQueryItem("hnd", QString::number(handle));
    query.addQueryItem("id", QString::number(id));
    query.addQueryItem("flag", QString::number(flag));
    query.addQueryItem("msg", data);
    query.addQueryItem("dlc", QString::number(dlc));

    baseUrl.setPath("/" + sessionId + "/canWrite");
    baseUrl.setQuery(query.query());

    QNetworkAccessManager *qnam = new QNetworkAccessManager;
    QNetworkReply *reply = qnam->get(QNetworkRequest(baseUrl));
    auto completion = [=]() {
        KvaserResponse res;

        if (reply->error()) {
            res.status = NETWORK_ERR;
            res.message = reply->errorString();
        } else {
            QJsonDocument jsonDoc = QJsonDocument::fromJson(reply->readAll());
            QJsonObject jsonObj = jsonDoc.object();
            res.status = (ResponseStatus)jsonObj["stat"].toInt();
            res.message = getMessageForStatus(res.status);
        }

        qnam->deleteLater();
        reply->deleteLater();
        callback(res);
    };
    QObject::connect(reply, &QNetworkReply::finished, this, completion);
}

void KvaserNetworkService::canRead(QString sessionId, int handle, int max, std::function<void (KvaserResponse, uint32_t, uint, std::vector<uint8_t>)> callback)
{
    QUrlQuery query;
    query.addQueryItem("hnd", QString::number(handle));
    query.addQueryItem("max", QString::number(max));

    baseUrl.setPath("/" + sessionId + "/canRead");
    baseUrl.setQuery(query.query());

    QNetworkAccessManager *qnam = new QNetworkAccessManager;
    QNetworkReply *reply = qnam->get(QNetworkRequest(baseUrl));
    auto completion = [=]() {
        KvaserResponse res;

        if (reply->error()) {
            res.status = NETWORK_ERR;
            res.message = reply->errorString();
            callback(res, 0, 0, {});
        } else {
            QJsonDocument jsonDoc = QJsonDocument::fromJson(reply->readAll());
            QJsonObject jsonObj = jsonDoc.object();

            res.status = (ResponseStatus)jsonObj["stat"].toInt();
            res.message = getMessageForStatus(res.status);

            foreach (const QJsonValue &jsonMessage, jsonObj["msgs"].toArray()) {
                QJsonObject message = jsonMessage.toObject();
                uint32_t id;
                uint flag;
                std::vector<uint8_t> data;

                id = message["id"].toInt();
                flag = message["flag"].toInt();
                foreach (const QJsonValue &byte, message["msg"].toArray()) {
                    data.push_back(byte.toInt());
                }

                callback(res, id, flag, data);
            }
        }

        qnam->deleteLater();
        reply->deleteLater();
    };
    QObject::connect(reply, &QNetworkReply::finished, this, completion);
}

void KvaserNetworkService::canBusOff(QString sessionId, int handle, std::function<void (KvaserResponse)> callback)
{
    QUrlQuery query;
    query.addQueryItem("hnd", QString::number(handle));

    baseUrl.setPath("/" + sessionId + "/canBusOff");
    baseUrl.setQuery(query.query());

    QNetworkAccessManager *qnam = new QNetworkAccessManager;
    QNetworkReply *reply = qnam->get(QNetworkRequest(baseUrl));
    auto completion = [=]() {
        KvaserResponse res;

        if (reply->error()) {
            res.status = NETWORK_ERR;
            res.message = reply->errorString();
        } else {
            QJsonDocument jsonDoc = QJsonDocument::fromJson(reply->readAll());
            QJsonObject jsonObj = jsonDoc.object();

            res.status = (ResponseStatus)jsonObj["stat"].toInt();
            res.message = getMessageForStatus(res.status);
        }

        qnam->deleteLater();
        reply->deleteLater();
        callback(res);
    };
    QObject::connect(reply, &QNetworkReply::finished, this, completion);
}

void KvaserNetworkService::closeChannel(QString sessionId, int handle, std::function<void (KvaserResponse)> callback)
{
    QUrlQuery query;
    query.addQueryItem("hnd", QString::number(handle));

    baseUrl.setPath("/" + sessionId + "/canClose");
    baseUrl.setQuery(query.query());

    QNetworkAccessManager *qnam = new QNetworkAccessManager;
    QNetworkReply *reply = qnam->get(QNetworkRequest(baseUrl));
    auto completion = [=]() {
        KvaserResponse res;

        if (reply->error()) {
            res.status = NETWORK_ERR;
            res.message = reply->errorString();
        } else {
            QJsonDocument jsonDoc = QJsonDocument::fromJson(reply->readAll());
            QJsonObject jsonObj = jsonDoc.object();

            res.status = (ResponseStatus)jsonObj["stat"].toInt();
            res.message = getMessageForStatus(res.status);
        }

        qnam->deleteLater();
        reply->deleteLater();
        callback(res);
    };
    QObject::connect(reply, &QNetworkReply::finished, this, completion);
}

void KvaserNetworkService::unloadLibrary(QString sessionId, std::function<void (KvaserResponse)> callback)
{
    baseUrl.setPath("/" + sessionId + "/canUnloadLibrary");

    QNetworkAccessManager *qnam = new QNetworkAccessManager;
    QNetworkReply *reply = qnam->get(QNetworkRequest(baseUrl));
    auto completion = [=]() {
        KvaserResponse res;

        if (reply->error()) {
            res.status = NETWORK_ERR;
            res.message = reply->errorString();
        } else {
            QJsonDocument jsonDoc = QJsonDocument::fromJson(reply->readAll());
            QJsonObject jsonObj = jsonDoc.object();

            res.status = (ResponseStatus)jsonObj["stat"].toInt();
            res.message = getMessageForStatus(res.status);
        }

        qnam->deleteLater();
        reply->deleteLater();
        callback(res);
    };
    QObject::connect(reply, &QNetworkReply::finished, this, completion);
}

