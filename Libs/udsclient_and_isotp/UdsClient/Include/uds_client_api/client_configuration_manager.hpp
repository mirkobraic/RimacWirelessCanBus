#pragma once
#include <map>
#include <thread>
#include <utility>
#include <vector>
#include "uds_client_api/client_configuration_structures.hpp"

namespace uds {
namespace client {
namespace configuration {
template <typename T>
class client_configuration_manager {
 public:
  virtual ~client_configuration_manager() = default;

  /**
   * @brief Get the rx tx pairs object
   *  This is the only function that the configuration provider must support
   * @returns List of rx-tx pairs
   */
  virtual std::vector<std::pair<T, T>> get_rx_tx_pairs() = 0;

  /**
   * @brief Get threadpool size for client
   *  If the thread count is set to lower than 2, it will be set to 2
   *  If this function is not implemented, min between hardware threadcount and 6 will be used
   * @returns number of threads in client threadpool
   */
  virtual uint8_t get_threadpool_size();

  /**
   * @brief Get the freeze frame read instructions object
   *  There is a default (empty) implementation of this function, it needs to be implemented only if this functionality
   * is needed
   * @return std::map<std::pair<T, T>, std::map<uint16_t, data_reader_instruction_for_multiple_fields>>
   */
  virtual std::map<std::pair<T, T>, std::map<uint16_t, uint16_t>> get_freeze_frame_read_instructions();

  /**
   * @brief Get the extended data read instructions object
   *  There is a default (empty) implementation of this function, it needs to be implemented only if this functionality
   * is needed
   * @return std::map<std::pair<T, T>, data_reader_instruction_for_multiple_fields>
   */
  virtual std::map<std::pair<T, T>, extended_data_reader_instruction_for_multiple_fields>
  get_extended_data_read_instructions();

  /**
   * @brief Get the data identifier read instructions object
   *  There is a default (empty) implementation of this function, it needs to be implemented only if this functionality
   * is needed
   * @return std::map<std::pair<T, T>, std::map<uint16_t, data_reader_instruction_for_single_did>>
   */
  virtual std::map<std::pair<T, T>, std::map<uint16_t, data_reader_instruction_for_single_did>>
  get_data_identifier_read_instructions();

  std::vector<uds_client_configuration<T>> get_client_configurations();
};
}  // namespace configuration
}  // namespace client
}  // namespace uds

using namespace uds::client::configuration;

template <typename T>
std::vector<uds_client_configuration<T>> client_configuration_manager<T>::get_client_configurations() {
  std::vector<uds_client_configuration<T>> result;

  auto available_rx_tx_pairs = get_rx_tx_pairs();
  auto freeze_frame_readers = get_freeze_frame_read_instructions();
  auto extended_data_readers = get_extended_data_read_instructions();
  auto data_identifier_readers = get_data_identifier_read_instructions();

  for (const auto& rx_tx : available_rx_tx_pairs) {
    uds_client_configuration<T> config;
    config.rx = rx_tx.first;
    config.tx = rx_tx.second;

    if (freeze_frame_readers.find(rx_tx) != freeze_frame_readers.end()) {
      config.freeze_frame_read_instructions = freeze_frame_readers[rx_tx];
    } else {
      config.freeze_frame_read_instructions = {};
    }
    if (extended_data_readers.find(rx_tx) != extended_data_readers.end()) {
      config.extended_data_read_instructions = extended_data_readers[rx_tx];
    } else {
      config.extended_data_read_instructions = {};
    }

    if (data_identifier_readers.find(rx_tx) != data_identifier_readers.end()) {
      config.data_identifier_read_instructions = data_identifier_readers[rx_tx];
    } else {
      config.data_identifier_read_instructions = {};
    }

    result.push_back(config);
  }

  return result;
}

template <typename T>
std::map<std::pair<T, T>, std::map<uint16_t, uint16_t>>
client_configuration_manager<T>::get_freeze_frame_read_instructions() {
  return std::map<std::pair<T, T>, std::map<uint16_t, uint16_t>>();
}

template <typename T>
std::map<std::pair<T, T>, extended_data_reader_instruction_for_multiple_fields>
client_configuration_manager<T>::get_extended_data_read_instructions() {
  return std::map<std::pair<T, T>, extended_data_reader_instruction_for_multiple_fields>();
}

template <typename T>
std::map<std::pair<T, T>, std::map<uint16_t, data_reader_instruction_for_single_did>>
client_configuration_manager<T>::get_data_identifier_read_instructions() {
  return std::map<std::pair<T, T>, std::map<uint16_t, data_reader_instruction_for_single_did>>();
}

template <typename T>
uint8_t client_configuration_manager<T>::get_threadpool_size() {
  // we don't use std::min here as it would require including algorithim and increasing compile time
  auto hw_threadcount = std::thread::hardware_concurrency();
  if (hw_threadcount > 4) {
    return (uint8_t)hw_threadcount;
  } else {
    return 4;
  }
}