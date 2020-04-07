#ifndef ISOTPMANAGER_H
#define ISOTPMANAGER_H

#include "IsotpLayer/IsotpCanProvider.h"
#include "IsotpLayer/IsotpLogger.h"

class IsotpManager
{

public:
    IsotpManager();

    void connect();
    void disconnect();
    void sendMessage(isotp::can_layer_message message);
    void switchCanProvider(CanBusProvider provider);

private:
    isotp_provider_handle* isotpHanlde = nullptr;
    std::shared_ptr<IsotpCanProvider> isotpCanProvider;
    std::shared_ptr<IsotpLogger> isotpLogger;

    std::vector<std::pair<uint32_t, uint32_t>> rxTxPairs;
};

#endif // ISOTPMANAGER_H
