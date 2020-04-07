#include "ViewController.h"

ViewController::ViewController(CanMessageListModel *recievedMessages, QObject *parent) : QObject(parent)
{
    canBusInterface = CanBusInterfaceFactory::getInterfaceForProvider(kvaser);
    this->recievedMessages = recievedMessages;
    QObject::connect(dynamic_cast<QObject*>(canBusInterface), SIGNAL(newDataFrame(CanMessage)), this, SLOT(dataFrameRecieved(CanMessage)), Qt::BlockingQueuedConnection);

    // ISOTP
//    isotpManager = IsotpManager();
}

ViewController::~ViewController()
{
    delete canBusInterface;
    delete recievedMessages;
}

void ViewController::connectTapped()
{
    canBusInterface->connect("Todo", Baud_500);
    recievedMessages->removeAll();
    isConnected = true;
    emit connectionChanged();

    // ISOTP
//    isotpManager.connect();
}

void ViewController::disconnectTapped()
{
    canBusInterface->disconnect();
    isConnected = false;
    emit connectionChanged();

    // ISOTP
//    isotpManager.disconnect();
}

void ViewController::sendTapped(QString messageId, QString messageData)
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

    // ISOTP
//    isotp::can_layer_message msg;
//    msg.id = id;
//    msg.data.assign(messageData.begin(), messageData.end());
//    isotpManager.sendMessage(msg);
}

void ViewController::dataFrameRecieved(CanMessage message)
{
    recievedMessages->addMessage(message);
}

bool ViewController::getIsConnected() const
{
    return isConnected;
}
