#include "IsotpManager.h"

IsotpManager::IsotpManager(CanBusProvider provider, std::vector<std::pair<uint32_t, uint32_t>> rxTxPairs, QObject *parent)
    : QObject(parent),
      canBusInterface(CanBusInterfaceFactory::getInterfaceForProvider(provider)),
      isotpCanProvider(std::make_shared<IsotpCanProvider>(canBusInterface)),
      isotpLogger(std::make_shared<IsotpLogger>()),
      isotpTransport(isotp::isotp_transport_layer(isotpCanProvider, rxTxPairs, isotpLogger))
{
    // QUESTION: what exacly are sender and recipient id and how do they relate to can message id
    isotpTransport.set_received_message_callback([this] (std::shared_ptr<uds::uds_message<uint32_t>> message) {
        try {
            CanMessage canMessage = CanMessage(message->recipient_id, message->data.size(), message->data);
            emit newMessageRecieved(canMessage);
        } catch (std::exception e) {
            qDebug() << "CanMessage constructor error:  " << e.what();
        }
    });
}

void IsotpManager::connect(std::string name, BaudRate baudRate)
{
    canBusInterface->connect(name, baudRate);
}

void IsotpManager::disconnect()
{
    canBusInterface->disconnect();
}

void IsotpManager::sendMessage(uds::uds_message<uint32_t> message)
{
    isotpTransport.send_message_to_peer(std::make_unique<uds::uds_message<uint32_t>>(message));
}
