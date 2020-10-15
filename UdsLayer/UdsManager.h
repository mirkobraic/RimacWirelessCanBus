#ifndef UDSMANAGER_H
#define UDSMANAGER_H

#include "UdsConfigManager.h"
#include "Logger.h"
#include "uds_client_api/u32_transport_layer/uds_client_with_u32_ids.hpp"

class UdsManager
{
public:
    static std::unique_ptr<uds_client::uds_client_with_u32_ids> makeUdsClient(std::shared_ptr<uds::transport_layer<uint32_t>> transport,
                                                                              std::pair<uint32_t, uint32_t> rxTxPairs,
                                                                              std::shared_ptr<Logger> logger);

private:
    UdsManager();
};

#endif // UDSMANAGER_H
