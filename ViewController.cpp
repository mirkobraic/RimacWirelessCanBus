#include "ViewController.h"

ViewController::ViewController(CanMessageListModel *recievedMessages, QObject *parent)
    : QObject(parent),
      recievedMessages(recievedMessages)
{ }

ViewController::~ViewController()
{
    delete recievedMessages;
    delete communicationManager;
}

void ViewController::connectTapped(int provider, int baudRate, const QVariantList& rxTxPairs)
{
    delete communicationManager;

    std::vector<std::pair<uint32_t, uint32_t>> pairs;
    for (const QVariant& pair : rxTxPairs) {
        QMap<QString, QVariant> pairMap = pair.toMap();
        pairs.push_back(std::make_pair(pairMap["rx"].toUInt(), pairMap["tx"].toUInt()));
    }

    communicationManager = new CommunicationManager((CanBusProvider)provider, pairs);
    QObject::connect(communicationManager, SIGNAL(newCanMessageRecieved(CanMessage)), this, SLOT(onNewCanMessageRecieved(CanMessage)), Qt::BlockingQueuedConnection);

    communicationManager->connect("172.20.10.3", (BaudRate)baudRate);
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

void ViewController::sendDirectCanMessage(QString messageId, const QVector<QString> &bytes)
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

    communicationManager->sendDirectCanMessage(data, id);
}

void ViewController::onNewCanMessageRecieved(CanMessage message)
{
    if (isRawCanEnabled) {
        recievedMessages->addMessage(message);
    }
}

void ViewController::checkVersion(int tx)
{
    communicationManager->udsCheckVersion(uint32_t(tx), [this] (QString title, QString message) {
        emit showAlert(title, message);
    });
}

bool ViewController::getIsConnected() const
{
    return isConnected;
}

bool ViewController::getIsRawCanEnabled() const
{
    return isRawCanEnabled;
}

void ViewController::setIsRawCanEnabled(bool value)
{
    isRawCanEnabled = value;
    emit isRawCanEnabledChanged();
}
