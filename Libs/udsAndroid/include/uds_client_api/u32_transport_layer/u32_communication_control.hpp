#pragma once
#include "uds_client_api/export_macro.hpp"
#include "uds_client_api/u32_transport_layer/u32_uds_client_provider_handle.hpp"
#include "uds_client_api/uds_return_types.hpp"

namespace uds_client {
class u32_communication_control final {
 public:
  explicit u32_communication_control(u32_uds_client_provider_handle* handle);

  /**
   * @brief Request 0x28, Subfunction 0x0
   * Sends request to server and requests from it to enable both sending and receiving certain messages
   * (specified by given communication type)
   *
   * @param tx address of the server
   * @param communication_type bit-code value which controls which messages the server should enable/disable
   * @return response indicating successful request, specific error or negative response otherwise
   */
  DLL_PUBLIC uds::response::uds_client_response<uds::response::positive_response>
  send_communication_control_enable_rx_and_tx(uint32_t tx, uint8_t communication_type);  // communication control, sub 0

  /**
   * @brief Request 0x28, Subfunction 0x1
   * Sends request to server and requests from it to enable receiving and disables sending certain messages
   * (specified by given communication type)
   *
   * @param tx address of the server
   * @param communication_type bit-code value which controls which messages the server should enable/disable
   * @return response indicating successful request, specific error or negative response otherwise
   */
  DLL_PUBLIC uds::response::uds_client_response<uds::response::positive_response>
  send_communication_control_enable_rx_and_disable_tx(uint32_t tx,
                                                      uint8_t communication_type);  // communication control, sub 1

  /**
   * @brief  Request 0x28, Subfunction 0x2
   *  Sends request to server and requests from it to enable receiving and disables receiving certain messages
   * (specified by given communication type)
   *
   * @param tx address of the server
   * @param communication_type bit-code value which controls which messages the server should enable/disable
   * @return response indicating successful request, specific error or negative response otherwise
   */
  DLL_PUBLIC uds::response::uds_client_response<uds::response::positive_response>
  send_communication_control_disable_rx_and_enable_tx(uint32_t tx,
                                                      uint8_t communication_type);  // communication control, sub 2

  /**
   * @brief  Request 0x28, Subfunction 0x3
   *  Sends request to server and requests from it to disable both sending and receiving certain messages
   * (specified by given communication type)
   *
   * @param tx address of the server
   * @param communication_type bit-code value which controls which messages the server should enable/disable
   * @return response indicating successful request, specific error or negative response otherwise
   */
  DLL_PUBLIC uds::response::uds_client_response<uds::response::positive_response>
  send_communication_control_disable_rx_and_tx(uint32_t tx,
                                               uint8_t communication_type);  // communication control, sub 3

 private:
  u32_uds_client_provider_handle* uds_client_handle;
};
}  // namespace uds_client