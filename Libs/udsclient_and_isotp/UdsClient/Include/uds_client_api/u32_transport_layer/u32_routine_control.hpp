#pragma once

#include "uds_client_api/export_macro.hpp"
#include "uds_client_api/u32_transport_layer/u32_uds_client_provider_handle.hpp"
#include "uds_client_api/uds_return_types.hpp"

namespace uds_client {
class u32_routine_control final {
 public:
  explicit u32_routine_control(u32_uds_client_provider_handle* handle);

  /**
   * @brief Request 0x31, Subfunction 0x01
   * Sends a request to start a routine
   *
   * @param tx address of the server
   * @param routine_identifier identifier of the routine to be started
   * @param routine_ctrl_option_record additional data to be provided to the routine
   * @return response indicating successful request, specific error or negative response otherwise
   */
  DLL_PUBLIC uds::response::uds_client_response<uds::response::positive_response>
  send_request_to_start_routine_on_server(
      uint32_t tx, uint16_t routine_identifier,
      std::vector<uint8_t> routine_ctrl_option_record);  // send_routine_control_request_with_subfunction_0x1

  /**
   * @brief Request 0x31, Subfunction 0x02
   * Sends a request to stop a routine
   *
   * @param tx address of the server
   * @param routine_identifier identifier of the routine to be stopped
   * @param routine_ctrl_option_record additional data to be provided to the routine
   * @return response indicating successful request, specific error or negative response otherwise
   */
  DLL_PUBLIC uds::response::uds_client_response<uds::response::positive_response>
  send_request_to_stop_routine_on_server(
      uint32_t tx, uint16_t routine_identifier,
      std::vector<uint8_t> routine_ctrl_option_record);  // send_routine_control_request_with_subfunction_0x2

  /**
   * @brief Request 0x31, Subfunction 0x03
   * Sends a request to get routine results
   *
   * @param tx address of the server
   * @param routine_identifier identfier of the routine to get results for
   * @return response containing routine results if request was successful, specific error or negative
   * response otherwise
   */
  DLL_PUBLIC uds::response::uds_client_response<std::vector<uint8_t>> send_request_to_get_routine_results(
      uint32_t tx,
      uint16_t routine_identifier);  // send_routine_control_request_with_subfunction_0x3

  /**
   * @brief Combines Request 0x31, subfunctions 0x01 and 0x03
   * Starts the routine and polls for its results
   *
   * @param tx address of the server
   * @param routine_identifier identifier of the routine
   * @param routine_ctrl_option_record additional data to be passed to the routine
   * @param poll_result_period period for sending get routine results after starting it
   * @param timeout_millis timeout for getting routine results
   * @return response containing routine results if request was successful, specific error or negative
   * response otherwise
   */
  DLL_PUBLIC uds::response::uds_client_response<std::vector<uint8_t>> start_routine_and_poll_results(
      uint32_t tx, uint16_t routine_identifier, const std::vector<uint8_t>& routine_ctrl_option_record,
      uint32_t poll_result_period, uint32_t timeout_millis);

 private:
  u32_uds_client_provider_handle* uds_client_handle;
};
}  // namespace uds_client