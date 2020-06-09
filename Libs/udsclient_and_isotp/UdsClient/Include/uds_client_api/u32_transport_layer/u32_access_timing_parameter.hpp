#pragma once

#include "uds_client_api/export_macro.hpp"
#include "uds_client_api/u32_transport_layer/u32_uds_client_provider_handle.hpp"
#include "uds_client_api/uds_return_types.hpp"

namespace uds_client {
class u32_access_timing_parameter final {
 public:
  explicit u32_access_timing_parameter(u32_uds_client_provider_handle* handle);

  /**
   * @brief Request 0x83, Subfunction 0x1
   * Function used to retrieve extended paramter for given tx
   *
   * @param tx address of the server
   * @return response containing extended timing parameter if request was successful, specific error or negative
   * response otherwise
   */
  DLL_PUBLIC uds::response::uds_client_response<uint16_t> read_extended_parameter_set(uint32_t tx);

  /**
   * @brief Request 0x83, Subfunction 0x2
   * Sets the timing parameters to default values
   *
   * @param tx address of the server
   * @return response indicating successful request, specific error or negative response otherwise
   */
  DLL_PUBLIC uds::response::uds_client_response<uds::response::positive_response>
  set_timing_parameters_to_default_values(uint32_t tx);

  /**
   * @brief Request 0x83, Subfunction 0x3
   * Get currently active standard and extended timing parameter
   *
   * @param tx address of the server
   * @return response containing currently active parameters if request was successful, specific error or negative
   * response otherwise
   */
  DLL_PUBLIC uds::response::uds_client_response<std::pair<uint16_t, uint16_t>> read_currently_active_parameters(
      uint32_t tx);

  /**
   * @brief Request 0x83, Subfunction 0x4
   * Set standard and extended timeout on server
   *
   * @param tx address of the server
   * @param standard_timeout value to which server should set the standard timeout
   * @param enhanced_timeout value to which server should set the extended timeout
   * @return response indicating successful request, specific error or negative response otherwise
   */
  DLL_PUBLIC uds::response::uds_client_response<uds::response::positive_response> set_timing_parameters_to_given_values(
      uint32_t tx, uint16_t standard_timeout, uint16_t enhanced_timeout);

 private:
  u32_uds_client_provider_handle* uds_client_handle;
};
}  // namespace uds_client