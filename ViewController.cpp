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

void ViewController::connectTapped(int provider, QString ipAddress, QString port, int baudRate, const QVariantMap& rxTxPair)
{
    // TODO: fix leak

    std::pair<uint32_t, uint32_t> pair;
    pair.first = rxTxPair.first().toString().toUInt(nullptr, 16);
    pair.second = rxTxPair.last().toString().toUInt(nullptr, 16);

    communicationManager = new CommunicationManager((CanBusProvider)provider, pair);
    QObject::connect(communicationManager, SIGNAL(newCanMessageRecieved(CanMessage)), this, SLOT(onNewCanMessageRecieved(CanMessage)));
    QObject::connect(communicationManager, SIGNAL(showAlert(QString, QString)), this, SLOT(onShowAlert(QString, QString)));
    QObject::connect(communicationManager, SIGNAL(fetchingInProgress(bool)), this, SLOT(onFetchingInProgress(bool)));
    QObject::connect(communicationManager, SIGNAL(toggleConnection(bool)), this, SLOT(onToggleConnection(bool)));

    communicationManager->connect(ipAddress, port, (BaudRate)baudRate);
    recievedMessages->removeAll();
    emit clearSupportedDtcs();
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
    recievedMessages->addMessage(message);
}

void ViewController::udsCheckVersion()
{
    QtConcurrent::run(communicationManager, &CommunicationManager::udsCheckVersion);
}

void ViewController::udsGetSupportedDtcsStatus()
{
    auto completion = [this] (const std::vector<int> &keys, const std::vector<int> &values) {
        QVector<int> qKeys = QVector<int>(keys.begin(), keys.end());
        QVector<int> qValues = QVector<int>(values.begin(), values.end());

        emit setSupportedDtcs(qKeys, qValues);
    };

    QtConcurrent::run(communicationManager, &CommunicationManager::udsGetSupportedDtcsStatus, completion);
}

void ViewController::udsClearDtcInformation()
{
    QtConcurrent::run(communicationManager, &CommunicationManager::udsClearDtcInformation);
}

void ViewController::onShowAlert(QString title, QString message)
{
    emit showAlert(title, message);
}

void ViewController::onFetchingInProgress(bool value)
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
