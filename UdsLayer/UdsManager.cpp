#include "UdsManager.h"

std::unique_ptr<uds_client::uds_client_with_u32_ids> UdsManager::makeUdsClient(std::shared_ptr<uds::transport_layer<uint32_t>> transport,
                                                                               std::vector<std::pair<uint32_t, uint32_t>> rxTxPairs,
                                                                               std::shared_ptr<Logger> logger)
{

    return std::make_unique<uds_client::uds_client_with_u32_ids>(transport,
                                                                 std::make_unique<UdsConfigManager>(rxTxPairs),
                                                                 logger);
}
