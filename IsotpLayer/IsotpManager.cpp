#include "IsotpManager.h"

std::shared_ptr<isotp::isotp_transport_layer> IsotpManager::makeTransportLayer(std::shared_ptr<CanBusInterface> canBusInterface,
                                                                               std::pair<uint32_t, uint32_t> rxTxPair,
                                                                               std::shared_ptr<Logger> logger)
{
    std::vector<std::pair<uint32_t, uint32_t>> pairVector = std::vector<std::pair<uint32_t, uint32_t>>();
    pairVector.push_back(rxTxPair);
    std::shared_ptr<IsotpCanProvider> isotpCanProvider = std::make_shared<IsotpCanProvider>(canBusInterface);
    std::shared_ptr<isotp::isotp_transport_layer> isotpTransport = std::make_shared<isotp::isotp_transport_layer>(isotpCanProvider,
                                                                                                                  pairVector,
                                                                                                                  logger);

    return isotpTransport;
}
