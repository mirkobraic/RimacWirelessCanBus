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

void CommunicationManager::sendMessage(uds::uds_message<uint32_t> msg)
{
    auto response = udsClient->check_version_servicees.send_check_version(msg.recipient_id);
    auto positiveResponse = [] (const std::pair<uds::version_params, uds::type_of_server> &pair) {
        qDebug() << "POSITIVE RESPONSE";
    };
    auto negativeResponse = [] (const uds::response::negative_response &res) {
        qDebug() << "NEGATIVE RESPONSE";
    };
    auto errorResponse = [] (const uds::response::response_error &res) {
        qDebug() << "RESPONSE ERROR";
    };
    response.unpack_response(positiveResponse, negativeResponse, errorResponse);
}

std::vector<std::pair<uint32_t, uint32_t> > CommunicationManager::getRxTxPairs() const
{
    return rxTxPairs;
}
