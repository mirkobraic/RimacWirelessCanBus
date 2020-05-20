#ifndef UDSCONFIGMANAGER_H
#define UDSCONFIGMANAGER_H

#include "uds_client_api/client_configuration_manager.hpp"

class UdsConfigManager : public client_configuration_manager<uint32_t>
{
public:
    UdsConfigManager(std::vector<std::pair<uint32_t, uint32_t>> rxTxPairs);

    void setRxTxPairs(const std::vector<std::pair<uint32_t, uint32_t>> &value);

    std::vector<std::pair<uint32_t, uint32_t>> get_rx_tx_pairs() override;

private:
    std::vector<std::pair<uint32_t, uint32_t>> rxTxPairs;
};

#endif // UDSCONFIGNMANAGER_H
