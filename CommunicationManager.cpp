#include "CommunicationManager.h"

CommunicationManager::CommunicationManager(CanBusProvider provider, std::vector<std::pair<uint32_t, uint32_t>> rxTxPairs, QObject *parent)
    : QObject(parent)
{
    this->rxTxPairs = rxTxPairs;
    logger = std::make_shared<Logger>();

    canBusInterface = CanBusInterfaceFactory::getInterfaceForProvider(provider);

    isotpTransport = IsotpManager::makeTransportLayer(canBusInterface, rxTxPairs, logger);
    isotpTransport->set_received_message_callback([this] (std::shared_ptr<uds::uds_message<uint32_t>> message) {
//        try {
//            CanMessage canMessage = CanMessage(message->recipient_id, message->data.size(), message->data);
//            emit newMessageRecieved(canMessage);
//        } catch (std::exception e) {
//            qDebug() << "CanMessage constructor error:  " << e.what();
//        }
        uds::uds_message<uint32_t> msg;
        msg.data = std::vector<uint8_t>() = {0x5, 0x41, 0x5, 0x4, 0x3, 0x0};
        msg.sender_id = 2;
        msg.recipient_id = 1;
        isotpTransport->send_message_to_peer(std::make_unique<uds::uds_message<uint32_t>>(msg));
    });

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
    auto response = udsClient->check_version_servicees.send_check_version(2);
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

//    isotpTransport->send_message_to_peer(std::make_unique<uds::uds_message<uint32_t>>(msg));
}
