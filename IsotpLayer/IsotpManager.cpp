#include "IsotpManager.h"

std::shared_ptr<isotp::isotp_transport_layer> IsotpManager::makeTransportLayer(std::shared_ptr<CanBusInterface> canBusInterface,
                                                                               std::vector<std::pair<uint32_t, uint32_t>> rxTxPairs,
                                                                               std::shared_ptr<Logger> logger)
{
    std::shared_ptr<IsotpCanProvider> isotpCanProvider = std::make_shared<IsotpCanProvider>(canBusInterface);
    std::shared_ptr<isotp::isotp_transport_layer> isotpTransport = std::make_shared<isotp::isotp_transport_layer>(isotpCanProvider,
                                                                                                                  rxTxPairs,
                                                                                                                  logger);

    return isotpTransport;
}
