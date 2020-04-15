#include "CanBusManager.h"

CanBusManager::CanBusManager(CanBusProvider provider, CanMessageListModel *recievedMessages, QObject *parent) : QObject(parent)
{
    canBusInterface = CanBusInterfaceFactory::getInterfaceForProvider(provider);
    this->recievedMessages = recievedMessages;
    QObject::connect(dynamic_cast<QObject*>(canBusInterface), SIGNAL(newDataFrame(CanMessage)), this, SLOT(dataFrameRecieved(CanMessage)), Qt::BlockingQueuedConnection);

}

CanBusManager::~CanBusManager()
{
    delete canBusInterface;
    delete recievedMessages;
}

void CanBusManager::connectTapped()
{
    canBusInterface->connect("Todo", Baud_1000);
    recievedMessages->removeAll();
    isConnected = true;
    emit connectionChanged();
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
    recievedMessages->addMessage(message);
}

bool CanBusManager::getIsConnected() const
{
    return isConnected;
}
