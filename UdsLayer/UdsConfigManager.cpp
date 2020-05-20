#include "UdsConfigManager.h"

UdsConfigManager::UdsConfigManager(std::vector<std::pair<uint32_t, uint32_t>> rxTxPairs)
{
    this->rxTxPairs = rxTxPairs;
}

void UdsConfigManager::setRxTxPairs(const std::vector<std::pair<uint32_t, uint32_t>> &value)
{
    rxTxPairs = value;
}

std::vector<std::pair<uint32_t, uint32_t>> UdsConfigManager::get_rx_tx_pairs()
{
    return rxTxPairs;
}
