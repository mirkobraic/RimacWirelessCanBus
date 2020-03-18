#include "CanBusManager.h"

#include <QDebug>

CanBusManager::CanBusManager(CanBusProvider provider, QObject *parent) : QObject(parent)
{
    canBusInterface = CanBusInterfaceFactory::getInterfaceForProvider(provider);
    QObject::connect(dynamic_cast<QObject*>(canBusInterface), SIGNAL(newDataFrame(CanMessage)), this, SLOT(dataFrameRecieved(CanMessage)));
}

void CanBusManager::connectTapped()
{
    connectionStatus = Connecting;
    emit connectionStatusChanged();

    canBusInterface->connect();

    connectionStatus = Connected;
    emit connectionStatusChanged();
}

void CanBusManager::disconnectTapped()
{
    canBusInterface->disconnect();
    connectionStatus = NotConnected;
    emit connectionStatusChanged();
}

void CanBusManager::sendTapped(QString messageId, QString messageData)
{
    bool success;
    uint id = messageId.toUInt(&success, 16);
    if (!success) {
        emit invalidCanId();
        return;
    }
    // TODO: allow spaces in data and maybe separate bytes
    messageData.toUInt(&success, 16);
    if (!success) {
        emit invalidCanData();
        return;
    }
    QByteArray data = QByteArray::fromHex(messageData.toUtf8());
    quint8 length = data.length();

    try {
        CanMessage message(id, length, data);
        canBusInterface->sendCanMessage(message);
    } catch (CanMessageException error) {
        switch (error) {
        case IdOutOfRange:
            emit invalidCanId();
            break;
        case DataOutOfRange:
            emit invalidCanData();
            break;
        }
    }
}

void CanBusManager::dataFrameRecieved(CanMessage message)
{
    QString hexId;
    hexId.setNum(message.getId(), 16);
    QString data = message.getData().toHex();
    emit addMessage(hexId, data);
}

CanBusManager::ConnectionStatus CanBusManager::getConnectionStatus() const
{
    return connectionStatus;
}
