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
        isConnected = true;
        emit connectionChanged();
    } catch (QString error) {
        qDebug() << error;
    }
}

void CanBusManager::disconnectTapped()
{
    canBusInterface->disconnect();
    isConnected = false;
    emit connectionChanged();
}

void CanBusManager::sendTapped(QString messageId, QString messageData)
{
    bool success;
    uint32_t id = messageId.toUInt(&success, 16);
    if (!success){
        qDebug() << "Invalid ID";
        return;
    }

    QRegularExpressionMatch match = hexMatcher.match(messageData);
    if (match.hasMatch() == false){
        qDebug() << "Invalid data";
        return;
    }

    QByteArray data = QByteArray::fromHex(messageData.toUtf8());
    uint8_t dlc = data.length();

    try {
        CanMessage message(id, dlc, data);
        canBusInterface->sendCanMessage(message);

    } catch (const std::exception& ex) {
        qDebug() << "Exception: " << ex.what();
    }
}

void CanBusManager::dataFrameRecieved(CanMessage message)
{
    QString hexId;
    hexId.setNum(message.getId(), 16);
    QString data = message.getData().toHex();
    emit addMessage(hexId, data);
}

bool CanBusManager::getIsConnected() const
{
    return isConnected;
}
