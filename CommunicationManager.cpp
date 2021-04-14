#include "CommunicationManager.h"

CommunicationManager::CommunicationManager(CanBusProvider provider, QObject *parent)
    : QObject(parent)
{
    canBusInterface = CanBusInterfaceFactory::getInterfaceForProvider(provider);
    QObject::connect(canBusInterface.get(), SIGNAL(showAlert(QString, QString)), this, SLOT(onShowAlert(QString, QString)));
    QObject::connect(canBusInterface.get(), SIGNAL(fetchingInProgress(bool)), this, SLOT(onFetchingInProgress(bool)));
    QObject::connect(canBusInterface.get(), SIGNAL(newDirectCanMessage(uint32_t, std::vector<uint8_t>)), this, SLOT(onNewDirectCanMessage(uint32_t, std::vector<uint8_t>)));
    QObject::connect(canBusInterface.get(), SIGNAL(toggleConnection(bool)), this, SLOT(onToggleConnection(bool)));

    timer.start();
}

void CommunicationManager::connect(QString ipAddress, QString port, BaudRate baudRate)
{
    canBusInterface->connectToDevice(ipAddress, port, baudRate);
}

void CommunicationManager::disconnect()
{
    canBusInterface->disconnectFromDevice();
}

void CommunicationManager::sendDirectCanMessage(std::vector<uint8_t> data, uint32_t id)
{
    CanMessage msg = CanMessage(id, data.size(), data);
    canBusInterface->sendCanMessage(msg);
}

void CommunicationManager::onNewDirectCanMessage(uint32_t id, std::vector<uint8_t> data)
{
    try {
        CanMessage canMessage = CanMessage(id, data.size(), data);
        canMessage.setTimestamp(timer.elapsed());
        emit newCanMessageRecieved(canMessage);
    } catch (std::exception e) {
        qDebug() << "CanMessage constructor error:  " << e.what();
    }
}

void CommunicationManager::onShowAlert(QString title, QString message)
{
    emit showAlert(title, message);
}

void CommunicationManager::onFetchingInProgress(bool value)
{
    emit fetchingInProgress(value);
}

void CommunicationManager::onToggleConnection(bool value)
{
    emit toggleConnection(value);
}
