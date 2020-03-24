#include "CanBusManager.h"

CanBusManager::CanBusManager(CanBusProvider provider, QObject *parent) : QObject(parent)
{
    canBusInterface = CanBusInterfaceFactory::getInterfaceForProvider(provider);
    QObject::connect(dynamic_cast<QObject*>(canBusInterface), SIGNAL(newDataFrame(CanMessage)), this, SLOT(dataFrameRecieved(CanMessage)));
}

void CanBusManager::connectTapped()
{
    try {
        canBusInterface->connect("Todo", Baud_500);
        connectionStatus = Connected;
        emit connectionStatusChanged();
    } catch (QString error) {
        qDebug() << error;
    }
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
    // TODO: improve check allow spaces in data and maybe separate bytes
    messageData.toLongLong(&success, 16);
    if (!success) {
        emit invalidCanData();
        return;
    }
    QByteArray data = QByteArray::fromHex(messageData.toUtf8());
    quint8 dlc = data.length();

    try {
        CanMessage message(id, dlc, data);
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
    qDebug() << "Recieved id: " << hexId << " data: " << message.getData();
    emit addMessage(hexId, data);
}

CanBusManager::ConnectionStatus CanBusManager::getConnectionStatus() const
{
    return connectionStatus;
}
