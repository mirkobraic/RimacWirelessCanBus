#include "KvaserNetworkService.h"

KvaserNetworkService::KvaserNetworkService(QObject *parent) : QObject(parent)
{

}

void KvaserNetworkService::setupDevice(QString ipAddress, QString port)
{
    baseUrl.setUrl("http://" + ipAddress + ":" + port);
}

void KvaserNetworkService::initializeLibrary(std::function<void (Status, QString)> callback)
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

        Status stat = (Status)jsobj["stat"].toInt();
        QString sessionId = jsobj["session"].toString();

        callback(stat, sessionId);
    };
    QObject::connect(reply, &QNetworkReply::finished, this, completion);
}

void KvaserNetworkService::openChannel(QString sessionId, std::function<void (KvaserNetworkService::Status, int)> callback)
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

        Status stat = (Status)jsobj["stat"].toInt();
        int handle = jsobj["hnd"].toInt();

        callback(stat, handle);
    };
    QObject::connect(reply, &QNetworkReply::finished, this, completion);
}

void KvaserNetworkService::setBusParams(QString sessionId, int handle, int baudRate, std::function<void (Status)> callback)
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
        Status stat = (Status)jsobj["stat"].toInt();

        callback(stat);
    };
    QObject::connect(reply, &QNetworkReply::finished, this, completion);
}

void KvaserNetworkService::canBusOn(QString sessionId, int handle, std::function<void (KvaserNetworkService::Status)> callback)
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
        Status stat = (Status)jsobj["stat"].toInt();

        callback(stat);
    };
    QObject::connect(reply, &QNetworkReply::finished, this, completion);
}

void KvaserNetworkService::canWrite(QString sessionId, int handle, uint32_t id, uint flag, QString data, size_t dlc, std::function<void (KvaserNetworkService::Status)> callback)
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
            return;
        }
        QJsonDocument jsdoc = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsobj = jsdoc.object();
        Status stat = (Status)jsobj["stat"].toInt();

        callback(stat);
    };
    QObject::connect(reply, &QNetworkReply::finished, this, completion);
}

void KvaserNetworkService::canRead(QString sessionId, int handle, std::function<void (KvaserNetworkService::Status, uint32_t, std::vector<uint8_t>)> callback)
{
    QUrlQuery query;
    query.addQueryItem("hnd", QString::number(handle));

    baseUrl.setPath("/" + sessionId + "/canRead");
    baseUrl.setQuery(query.query());

    QNetworkAccessManager networkManager;
    QNetworkReply *reply = networkManager.get(QNetworkRequest(baseUrl));
    auto completion = [=]() {
        if (reply->error()) {
            qDebug() << reply->errorString();
            return;
        }

        QJsonDocument jsdoc = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsobj = jsdoc.object();
        Status stat = (Status)jsobj["stat"].toInt();

        QJsonArray messages = jsobj["msgs"].toArray();
        foreach (const QJsonValue &messageJson, messages) {
            QJsonObject message = messageJson.toObject();
            uint32_t id;
            id = message["id"].toInt();

            QJsonArray bytes = jsobj["msg"].toArray();
            std::vector<uint8_t> data;
            foreach (const QJsonValue &byte, bytes) {
                data.push_back(byte.toInt());
            }

            callback(stat, id, data);
        }

    };
    QObject::connect(reply, &QNetworkReply::finished, this, completion);
}

void KvaserNetworkService::canBusOff(QString sessionId, int handle, std::function<void (KvaserNetworkService::Status)> callback)
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
        Status stat = (Status)jsobj["stat"].toInt();

        callback(stat);
    };
    QObject::connect(reply, &QNetworkReply::finished, this, completion);
}

void KvaserNetworkService::closeChannel(QString sessionId, int handle, std::function<void (Status)> callback)
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
        Status stat = (Status)jsobj["stat"].toInt();

        callback(stat);
    };
    QObject::connect(reply, &QNetworkReply::finished, this, completion);
}

void KvaserNetworkService::unloadLibrary(QString sessionId, std::function<void (Status)> callback)
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
        Status stat = (Status)jsobj["stat"].toInt();

        callback(stat);
    };
    QObject::connect(reply, &QNetworkReply::finished, this, completion);
}

