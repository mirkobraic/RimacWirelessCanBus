#pragma once

#include "uds_client_api/export_macro.hpp"
#include "uds_client_api/u32_transport_layer/u32_uds_client_provider_handle.hpp"
#include "uds_client_api/uds_return_types.hpp"

namespace uds_client {
class u32_ecu_reset final {
 public:
  explicit u32_ecu_reset(u32_uds_client_provider_handle* handle);

  /**
   * @brief Reqeust 0x11, Subfunction 0x03
   * Requests to perform soft ECU reset
   *
   * @param tx address of the server
   * @return response indicating successful request, specific error or negative response otherwise
   */
  DLL_PUBLIC uds::response::uds_client_response<uds::response::positive_response> send_ecu_soft_reset(uint32_t tx);

  /**
   * @brief Reqeust 0x11, Subfunction 0x01
   * Requests to perform hardECU reset
   *
   * @param tx address of the server
   * @return response indicating successful request, specific error or negative response otherwise
   */
  DLL_PUBLIC uds::response::uds_client_response<uds::response::positive_response> send_ecu_hard_reset(uint32_t tx);

  /**
   * @brief Reqeust 0x11, Subfunction 0x02
   * Requests to perform key on-off ECU reset
   *
   * @param tx address of the server
   * @return response indicating successful request, specific error or negative response otherwise
   */
  DLL_PUBLIC uds::response::uds_client_response<uds::response::positive_response> perform_key_off_on_reset(uint32_t tx);

  /**
   * @brief Reqeust 0x11, Subfunction 0x04
   * Requests to enable rapid  power shut down ECU reset
   *
   * @param tx address of the server
   * @return response containing rapid power shutdown timer if request was successful, specific error or negative
   * response otherwise
   */
  DLL_PUBLIC uds::response::uds_client_response<uint8_t> enable_rapid_power_shut_down(uint32_t tx);

  /**
   * @brief Reqeust 0x11, Subfunction 0x05
   * Requests to disable rapid power shut down
   *
   * @param tx address of the server
   * @return response indicating successful request, specific error or negative response otherwise
   */
  DLL_PUBLIC uds::response::uds_client_response<uds::response::positive_response> disable_rapid_power_shut_down(
      uint32_t tx);

 private:
  u32_uds_client_provider_handle* uds_client_handle;
};
}  // namespace uds_client