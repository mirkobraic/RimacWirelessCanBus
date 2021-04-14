#pragma once

#include <functional>
#include <map>
#include "uds_client_api/export_macro.hpp"
#include "uds_client_api/u32_transport_layer/u32_uds_client_provider_handle.hpp"
#include "uds_client_api/uds_return_types.hpp"

namespace uds_client {
class u32_data_transfer final {
 public:
  explicit u32_data_transfer(u32_uds_client_provider_handle *handle);

  /**
   * @brief  Requests 0x34 and 0x36 combined in single call
   * Used for negotiating and sending data from client to server
   *
   * @param tx address of the server
   * @param memory_address address to which to write the data
   * @param memory_size size that will be written
   * @param transfer_request_parameter_record data to be written
   * @param progress_callback callback to be called while sending data with updated percentage sent
   * @return response indicating successful request, specific error or negative response otherwise
   */
  DLL_PUBLIC uds::response::uds_client_response<uds::response::positive_response> send_download_service_request(
      uint32_t tx, uint32_t memory_address, uint32_t memory_size,
      const std::vector<uint8_t> &transfer_request_parameter_record, std::function<void(float)> &progress_callback,
      uint8_t data_format_identifier = 0x0);

  /**
   * @brief  Requests 0x35 and 0x36 combined in single call
   * Used for negotiating and retrieving data from server to client
   *
   * @param tx address of the server
   * @param memory_address address from which to read the data
   * @param memory_size size that will be read
   * @return response containing read data if request was successful, specific error or negative
   * response otherwise
   */
  DLL_PUBLIC uds::response::uds_client_response<std::vector<uint8_t>> send_upload_service_request(
      uint32_t tx, uint32_t memory_address, uint32_t memory_size);

  /**
   * @brief Request 0x22, Subfunction N/A
   * Used for reading data by identifier
   * @param tx address of the server
   * @param identifiers_of_interest identifiers whose data needs to be retrieved from the server
   * @return response containing read data if request was successful, specific error or negative
   * response otherwise
   */
  DLL_PUBLIC uds::response::uds_client_response<std::map<uint16_t, std::vector<uint8_t>>>
  send_read_data_by_identifier_request(uint32_t tx, std::vector<uint16_t> identifiers_of_interest);

  /**
   * @brief Request 0x22, Subfunction N/A
   * Used for reading data by identifier (with unparsed response)
   * @param tx address of the server
   * @param identifiers_of_interest identifiers whose data needs to be retrieved from the server
   * @return response containing raw bytes of response if request was successful, specific error or negative
   * response otherwise
   */
  DLL_PUBLIC uds::response::uds_client_response<std::vector<uint8_t>>
  send_read_data_by_identifier_service_request_raw_response(uint32_t tx, std::vector<uint16_t> identifiers_of_interest);

  /**
   * @brief Request 0x23, Subfunction N/A
   * Used for reading memory by address
   * @param tx address of the server
   * @param address_and_length_format_identifier address and length format identifer
   * @param memory_address address for reading from
   * @param memory_size Length of data to be read
   * @return response containing read data if request was successful, specific error or negative
   * response otherwise
   */
  DLL_PUBLIC uds::response::uds_client_response<std::vector<uint8_t>> send_read_memory_by_address_request(
      uint32_t tx, uint8_t address_and_length_format_identifier, uint32_t memory_address, uint32_t memory_size);

  /**
   * @brief Request 0x24, Subfunction N/A
   * Used for reading scaling data by identifier
   * @param tx address of the server
   * @param data_identifier identifier of the data that needs to be retrieved
   * @return response containing read data if request was successful, specific error or negative
   * response otherwise
   */
  DLL_PUBLIC uds::response::uds_client_response<std::pair<uint16_t, std::vector<uint8_t>>>
  send_read_scaling_data_by_identifier_request(uint32_t tx, uint16_t data_identifier);

  /**
   * @brief Request 0x2E, Subfunction N/A
   * Used for writing data by identifier
   * @param tx address of the server
   * @param data_identifier identifier of the data that needs to be written to
   * @param data_record data that needs to be written
   * @return response indicating successful request, specific error or negative response otherwise
   */
  DLL_PUBLIC uds::response::uds_client_response<uds::response::positive_response> send_write_data_by_identifier_request(
      uint32_t tx, uint16_t data_identifier, const std::vector<uint8_t> &data_record);

  /**
   * @brief Request 0x3D, Subfunction N/A
   * Used for writing data by address
   * @param tx address of the server
   * @param address_and_length_format_identifier address and length format identifier
   * @param memory_address address that needs to be written to
   * @param data_record data that needs to be written
   * @return response indicating successful request, specific error or negative response otherwise
   */
  DLL_PUBLIC uds::response::uds_client_response<uds::response::positive_response> send_write_memory_by_address_request(
      uint32_t tx, uint8_t address_and_length_format_identifier, uint32_t memory_address,
      const std::vector<uint8_t> &data_record);

 private:
  u32_uds_client_provider_handle *uds_client_handle;
};
}  // namespace uds_client