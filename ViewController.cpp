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

void ViewController::connectTapped(int provider, QString ipAddress, QString port, int baudRate, const QVariantList& rxTxPairs)
{
    // TODO: fix leak

    std::vector<std::pair<uint32_t, uint32_t>> pairs;
    for (const QVariant& pair : rxTxPairs) {
        QMap<QString, QVariant> pairMap = pair.toMap();
        pairs.push_back(std::make_pair(pairMap["rx"].toUInt(), pairMap["tx"].toUInt()));
    }

    communicationManager = new CommunicationManager((CanBusProvider)provider, pairs);
    QObject::connect(communicationManager, SIGNAL(newCanMessageRecieved(CanMessage)), this, SLOT(onNewCanMessageRecieved(CanMessage)));
    QObject::connect(communicationManager, SIGNAL(showAlert(QString, QString)), this, SLOT(onShowAlert(QString, QString)));
    QObject::connect(communicationManager, SIGNAL(toggleBusyIndicator(bool)), this, SLOT(onToggleBusyIndicator(bool)));
    QObject::connect(communicationManager, SIGNAL(toggleConnection(bool)), this, SLOT(onToggleConnection(bool)));

    communicationManager->connect(ipAddress, port, (BaudRate)baudRate);
    recievedMessages->removeAll();
}

void ViewController::disconnectTapped()
{
    communicationManager->disconnect();
}

void ViewController::sendDirectCanMessage(QString messageId, const QVector<QString> &bytes)
{
    bool success;
    uint32_t id = messageId.toUInt(&success, 16);
    if (!success){
        qDebug() << "Parsing qml error: Invalid ID";
        emit showAlert("Invalid ID", "");
        return;
    }

    std::vector<uint8_t> data;
    QVectorIterator<QString> it(bytes);
    while (it.hasNext()) {
        QString stringByte = it.next();
        uint8_t byte = stringByte.toUInt(&success, 16);
        if (!success){
            qDebug() << "Parsing qml error: Invalid data";
            emit showAlert("Invalid Data", "");
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
    QtConcurrent::run(communicationManager, &CommunicationManager::udsCheckVersion, tx);
}

void ViewController::onShowAlert(QString title, QString message)
{
    emit showAlert(title, message);
}

void ViewController::onToggleBusyIndicator(bool value)
{
    fetchingInProgress = value;
    emit fetchingInProgressChanged();
}

void ViewController::onToggleConnection(bool value)
{
    isConnected = value;
    emit connectionChanged();
}

bool ViewController::getFetchingInProgress() const
{
    return fetchingInProgress;
}

void ViewController::setFetchingInProgress(bool value)
{
    fetchingInProgress = value;
    emit fetchingInProgressChanged();
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
