#include "CommunicationManager.h"

CommunicationManager::CommunicationManager(CanBusProvider provider, std::vector<std::pair<uint32_t, uint32_t>> rxTxPairs, QObject *parent)
    : QObject(parent)
{
    this->rxTxPairs = rxTxPairs;
    logger = std::make_shared<Logger>();

    canBusInterface = CanBusInterfaceFactory::getInterfaceForProvider(provider);
    QObject::connect(canBusInterface.get(), SIGNAL(showAlert(QString, QString)), this, SLOT(onShowAlert(QString, QString)));
    canBusInterface->messageRecievedDirectCallback = [this] (uint32_t id, std::vector<uint8_t> data) {
        try {
            CanMessage canMessage = CanMessage(id, data.size(), data);
            emit newCanMessageRecieved(canMessage);
        } catch (std::exception e) {
            qDebug() << "CanMessage constructor error:  " << e.what();
        }
    };

    isotpTransport = IsotpManager::makeTransportLayer(canBusInterface, rxTxPairs, logger);

    udsClient = UdsManager::makeUdsClient(isotpTransport, rxTxPairs, logger);
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
    isotp::can_layer_message msg;
    msg.data = data;
    msg.id = id;
    canBusInterface->sendCanMessage(msg);
}

void CommunicationManager::udsCheckVersion(uint32_t tx)
{
    auto response = udsClient->check_version_servicees.send_check_version(tx);
    auto positiveResponse = [this] (const std::pair<uds::version_params, uds::type_of_server> &pair) {
        QString message = "Version params: " + QString::number(pair.first.major) + "." + QString::number(pair.first.minor) + "." + QString::number(pair.first.patch);
        message += "\nType of server: " + QString::number((int)pair.second);
        emit showAlert("Success", message);
    };
    auto negativeResponse = [this] (const uds::response::negative_response &res) {
        QString message = "code: " + QString::number((int)res);
        emit showAlert("negative", message);
    };
    auto errorResponse = [this] (const uds::response::response_error &res) {
        QString message = "code: " + QString::number((int)res);
        emit showAlert("Error", message);
    };
    response.unpack_response(positiveResponse, negativeResponse, errorResponse);
}

void CommunicationManager::onShowAlert(QString title, QString message)
{
    emit showAlert(title, message);
}
