#include "CommunicationManager.h"

CommunicationManager::CommunicationManager(CanBusProvider provider, std::vector<std::pair<uint32_t, uint32_t>> rxTxPairs, QObject *parent)
    : QObject(parent)
{
    this->rxTxPairs = rxTxPairs;
    logger = std::make_shared<Logger>();

    canBusInterface = CanBusInterfaceFactory::getInterfaceForProvider(provider);

    isotpTransport = IsotpManager::makeTransportLayer(canBusInterface, rxTxPairs, logger);

    udsClient = UdsManager::makeUdsClient(isotpTransport, rxTxPairs, logger);
}

void CommunicationManager::connect(std::string name, BaudRate baudRate)
{
    canBusInterface->connect(name, baudRate);
}

void CommunicationManager::disconnect()
{
    canBusInterface->disconnect();
}

void CommunicationManager::sendDirectCanMessage(std::vector<uint8_t> data, uint32_t id)
{
    isotp::can_layer_message msg;
    msg.data = data;
    msg.id = id;
    canBusInterface->sendCanMessage(msg);
}

void CommunicationManager::udsCheckVersion(uint32_t tx, std::function<void(QString, QString)> callback)
{
    auto response = udsClient->check_version_servicees.send_check_version(tx);
    auto positiveResponse = [callback] (const std::pair<uds::version_params, uds::type_of_server> &pair) {
        QString message = "Version params: " + QString::number(pair.first.major) + "." + QString::number(pair.first.minor) + "." + QString::number(pair.first.patch);
        message += "\nType of server: " + QString::number((int)pair.second);
        callback("Success", message);
    };
    auto negativeResponse = [callback] (const uds::response::negative_response &res) {
        QString message = "code: " + QString::number((int)res);
        callback("negative", message);
    };
    auto errorResponse = [callback] (const uds::response::response_error &res) {
        QString message = "code: " + QString::number((int)res);
        callback("Error", message);
    };
    response.unpack_response(positiveResponse, negativeResponse, errorResponse);
}
