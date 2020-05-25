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

void KvaserNetworkService::initializeLibrary(std::function<void (ResponseStatus, QString)> callback)
{
    baseUrl.setPath("/canInitializeLibrary");
    // TODO: used in development
    QUrlQuery query;
    query.addQueryItem("timeout", QString::number(10));
    baseUrl.setQuery(query.query());

    QNetworkReply *reply = networkManager.get(QNetworkRequest(baseUrl));
    auto completion = [=]() {
        if (reply->error()) {
            qDebug() << reply->errorString();
            reply->deleteLater();
            return;
        }

        QJsonDocument jsonDoc = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsonObj = jsonDoc.object();

        ResponseStatus stat = (ResponseStatus)jsonObj["stat"].toInt();
        QString sessionId = jsonObj["session"].toString();

        reply->deleteLater();
        callback(stat, sessionId);
    };
    QObject::connect(reply, &QNetworkReply::finished, this, completion);
}

void KvaserNetworkService::openChannel(QString sessionId, int channel, OpenFlags flags, std::function<void (ResponseStatus, int)> callback)
{
    QUrlQuery query;
    query.addQueryItem("channel", QString::number(channel));
    query.addQueryItem("flags", QString::number(flags));

    baseUrl.setPath("/" + sessionId + "/canOpenChannel");
    baseUrl.setQuery(query.query());

    QNetworkReply *reply = networkManager.get(QNetworkRequest(baseUrl));
    auto completion = [=]() {
        if (reply->error()) {
            qDebug() << reply->errorString();
            reply->deleteLater();
            return;
        }

        QJsonDocument jsonDoc = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsonObj = jsonDoc.object();

        ResponseStatus stat = (ResponseStatus)jsonObj["stat"].toInt();
        int handle = jsonObj["hnd"].toInt();

        reply->deleteLater();
        callback(stat, handle);
    };
    QObject::connect(reply, &QNetworkReply::finished, this, completion);
}

void KvaserNetworkService::setBaudRate(QString sessionId, int handle, BaudRate baudRate, std::function<void (ResponseStatus)> callback)
{
    QUrlQuery query;
    query.addQueryItem("hnd", QString::number(handle));
    query.addQueryItem("freq", QString::number(getKvaserBaudRate(baudRate)));

    baseUrl.setPath("/" + sessionId + "/canSetBusParams");
    baseUrl.setQuery(query.query());

    QNetworkReply *reply = networkManager.get(QNetworkRequest(baseUrl));
    auto completion = [=]() {
        if (reply->error()) {
            qDebug() << reply->errorString();
            reply->deleteLater();
            return;
        }

        QJsonDocument jsonDoc = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsonObj = jsonDoc.object();

        ResponseStatus stat = (ResponseStatus)jsonObj["stat"].toInt();

        reply->deleteLater();
        callback(stat);
    };
    QObject::connect(reply, &QNetworkReply::finished, this, completion);
}

void KvaserNetworkService::canBusOn(QString sessionId, int handle, std::function<void (ResponseStatus)> callback)
{
    QUrlQuery query;
    query.addQueryItem("hnd", QString::number(handle));

    baseUrl.setPath("/" + sessionId + "/canBusOn");
    baseUrl.setQuery(query.query());

    QNetworkReply *reply = networkManager.get(QNetworkRequest(baseUrl));
    auto completion = [=]() {
        if (reply->error()) {
            qDebug() << reply->errorString();
            reply->deleteLater();
            return;
        }
        QJsonDocument jsonDoc = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsonObj = jsonDoc.object();

        ResponseStatus stat = (ResponseStatus)jsonObj["stat"].toInt();

        reply->deleteLater();
        callback(stat);
    };
    QObject::connect(reply, &QNetworkReply::finished, this, completion);
}

void KvaserNetworkService::canWrite(QString sessionId, int handle, uint32_t id, uint flag, QString data, size_t dlc, std::function<void (ResponseStatus)> callback)
{
    QUrlQuery query;
    query.addQueryItem("hnd", QString::number(handle));
    query.addQueryItem("id", QString::number(id));
    query.addQueryItem("flag", QString::number(flag));
    query.addQueryItem("msg", data);
    query.addQueryItem("dlc", QString::number(dlc));

    baseUrl.setPath("/" + sessionId + "/canWrite");
    baseUrl.setQuery(query.query());

    QNetworkReply *reply = networkManager.get(QNetworkRequest(baseUrl));
    auto completion = [=]() {
        if (reply->error()) {
            qDebug() << reply->errorString();
            reply->deleteLater();
            return;
        }
        QJsonDocument jsonDoc = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsonObj = jsonDoc.object();
        ResponseStatus stat = (ResponseStatus)jsonObj["stat"].toInt();

        reply->deleteLater();
        callback(stat);
    };
    QObject::connect(reply, &QNetworkReply::finished, this, completion);
}

void KvaserNetworkService::canRead(QString sessionId, int handle, int max, std::function<void (ResponseStatus, uint32_t, uint, std::vector<uint8_t>)> callback)
{
    QUrlQuery query;
    query.addQueryItem("hnd", QString::number(handle));
    query.addQueryItem("max", QString::number(max));

    baseUrl.setPath("/" + sessionId + "/canRead");
    baseUrl.setQuery(query.query());

    QNetworkReply *reply = networkManager.get(QNetworkRequest(baseUrl));
    auto completion = [=]() {
        if (reply->error()) {
            qDebug() << reply->errorString();
            reply->deleteLater();
            return;
        }

        QJsonDocument jsonDoc = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsonObj = jsonDoc.object();

        ResponseStatus stat = (ResponseStatus)jsonObj["stat"].toInt();

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

            reply->deleteLater();
            callback(stat, id, flag, data);
        }

    };
    QObject::connect(reply, &QNetworkReply::finished, this, completion);
}

void KvaserNetworkService::canBusOff(QString sessionId, int handle, std::function<void (ResponseStatus)> callback)
{
    QUrlQuery query;
    query.addQueryItem("hnd", QString::number(handle));

    baseUrl.setPath("/" + sessionId + "/canBusOff");
    baseUrl.setQuery(query.query());

    QNetworkReply *reply = networkManager.get(QNetworkRequest(baseUrl));
    auto completion = [=]() {
        if (reply->error()) {
            qDebug() << reply->errorString();
            reply->deleteLater();
            return;
        }

        QJsonDocument jsonDoc = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsonObj = jsonDoc.object();

        ResponseStatus stat = (ResponseStatus)jsonObj["stat"].toInt();

        reply->deleteLater();
        callback(stat);
    };
    QObject::connect(reply, &QNetworkReply::finished, this, completion);
}

void KvaserNetworkService::closeChannel(QString sessionId, int handle, std::function<void (ResponseStatus)> callback)
{
    QUrlQuery query;
    query.addQueryItem("hnd", QString::number(handle));

    baseUrl.setPath("/" + sessionId + "/canClose");
    baseUrl.setQuery(query.query());

    QNetworkReply *reply = networkManager.get(QNetworkRequest(baseUrl));
    auto completion = [=]() {
        if (reply->error()) {
            qDebug() << reply->errorString();
            reply->deleteLater();
            return;
        }
        QJsonDocument jsonDoc = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsonObj = jsonDoc.object();

        ResponseStatus stat = (ResponseStatus)jsonObj["stat"].toInt();

        reply->deleteLater();
        callback(stat);
    };
    QObject::connect(reply, &QNetworkReply::finished, this, completion);
}

void KvaserNetworkService::unloadLibrary(QString sessionId, std::function<void (ResponseStatus)> callback)
{
    baseUrl.setPath("/" + sessionId + "/canUnloadLibrary");

    QNetworkReply *reply = networkManager.get(QNetworkRequest(baseUrl));
    auto completion = [=]() {
        if (reply->error()) {
            qDebug() << reply->errorString();
            reply->deleteLater();
            return;
        }
        QJsonDocument jsonDoc = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsonObj = jsonDoc.object();

        ResponseStatus stat = (ResponseStatus)jsonObj["stat"].toInt();

        reply->deleteLater();
        callback(stat);
    };
    QObject::connect(reply, &QNetworkReply::finished, this, completion);
}

