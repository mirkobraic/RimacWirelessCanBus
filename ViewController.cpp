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
    uint8_t dlc = data.size();

    try {
        CanMessage message(id, dlc, data);
        canBusInterface->sendCanMessage(message);

    } catch (const std::exception& ex) {
        qDebug() << "Exception: " << ex.what();
    }

    // ISOTP
//    isotp_message msg;
//    msg.id = id;
//    std::string messageString = messageData.toStdString();
//    msg.data.assign(messageString.begin(), messageString.end());
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
