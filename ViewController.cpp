#include "ViewController.h"

ViewController::ViewController(CanMessageListModel *recievedMessages, QObject *parent)
    : QObject(parent),
      recievedMessages(recievedMessages)
{
    std::vector<std::pair<uint32_t, uint32_t>> rxTxPairs;
    rxTxPairs.push_back(std::make_pair(rx, tx));
    communicationManager = new CommunicationManager(kvaser, rxTxPairs);

    QObject::connect(communicationManager, SIGNAL(newMessageRecieved(CanMessage)), this, SLOT(recievedMessageHandler(CanMessage)), Qt::BlockingQueuedConnection);
}

ViewController::~ViewController()
{
    delete recievedMessages;
    delete communicationManager;
}

void ViewController::connectTapped()
{
    communicationManager->connect("Todo", Baud_500);
    recievedMessages->removeAll();
    isConnected = true;
    emit connectionChanged();
}

void ViewController::disconnectTapped()
{
    communicationManager->disconnect();
    isConnected = false;
    emit connectionChanged();
}

void ViewController::sendTapped(QString messageId, const QVector<QString> &bytes)
{
    bool success;
    uint32_t id = messageId.toUInt(&success, 16);
    if (!success){
        qDebug() << "Parsing qml error: Invalid ID";
        return;
    }

    std::vector<uint8_t> data;
    QVectorIterator<QString> it(bytes);
    while (it.hasNext()) {
        QString stringByte = it.next();
        uint8_t byte = stringByte.toUInt(&success, 16);
        if (!success){
            qDebug() << "Parsing qml error: Invalid data";
            return;
        }
       data.push_back(byte);
    }

    uds::uds_message<uint32_t> msg;
    msg.data = data;
    msg.sender_id = rx;
    msg.recipient_id = tx;
    communicationManager->sendMessage(msg);
}

void ViewController::recievedMessageHandler(CanMessage message)
{
    recievedMessages->addMessage(message);
}

bool ViewController::getIsConnected() const
{
    return isConnected;
}
