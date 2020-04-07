#include "IsotpManager.h"

IsotpManager::IsotpManager()
{
    isotpCanProvider = std::make_shared<IsotpCanProvider>(kvaser);
    isotpLogger = std::make_shared<IsotpLogger>();
    rxTxPairs.push_back(std::make_pair(1, 2));
}

void IsotpManager::connect()
{
    isotpCanProvider->connect_to_can("Todo", Baud_500);
//    isotpHanlde = construct_isotp_provider_handle(isotpCanProvider, rxTxPairs, isotpLogger);
}

void IsotpManager::disconnect()
{
    isotpCanProvider->disconnect_from_can();
//    destruct_isotp_provider_handle(isotpHanlde);
}

void IsotpManager::sendMessage(isotp::can_layer_message message)
{
    auto msg = std::make_shared<isotp::can_layer_message>(message);
//    send_isotp_message_over_provider_handle(isotpHanlde, &msg);
}

void IsotpManager::switchCanProvider(CanBusProvider provider)
{
    isotpCanProvider->setCanProvider(provider);
}
