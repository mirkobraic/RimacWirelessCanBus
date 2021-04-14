#pragma once

#include <functional>
#include <optional>
#include "uds_client_api/client_services_structures.hpp"
#include "uds_client_api/export_macro.hpp"
#include "uds_client_api/u32_transport_layer/u32_uds_client_provider_handle.hpp"
#include "uds_client_api/uds_return_types.hpp"

namespace uds_client {
class u32_security_levels_and_sessions final {
 public:
  explicit u32_security_levels_and_sessions(u32_uds_client_provider_handle* handle);

  /**
   * @brief Request 0x10, subfunction N/A
   * Sends a request to swith to session
   *
   * @param tx address of the server
   * @param session session to switch to
   * @return response indicating successful request, specific error or negative response otherwise
   */
  DLL_PUBLIC uds::response::uds_client_response<uds::response::positive_response> send_switch_to_session(
      uint32_t tx, uint8_t session);

  /**
   * @brief Request 0x10, subfunction N/A
   * Sends a request to swith to session
   *
   * @param tx address of the server
   * @param session session to switch to
   * @return response indicating successful request, specific error or negative response otherwise
   */
  DLL_PUBLIC uds::response::uds_client_response<uds_client::timeouts> send_switch_to_session_and_get_timeouts(
      uint32_t tx, uint8_t session);

  /**
   * @brief Gets the active session of wanted server. Note that this is not an UDS request, but rather the last session
   * the client has requested the server to switch to
   *
   * @param tx address of the server
   * @warning This is not an UDS request, but rather the last session the client has requested the server to switch to
   * @return response containing last session if requested TX is supported, error otherwise
   */
  DLL_PUBLIC uds::response::uds_client_response<uint8_t> get_active_session(uint32_t tx);

  /**
   * @brief Request 0x27, Subfunction N/A
   * Requests the server to switch to level using xcp algorithm for calculating key from seed
   * @param tx address of the server
   * @param level wanted level
   * @return response indicating successful request, specific error or negative response otherwise
   */
  DLL_PUBLIC uds::response::uds_client_response<uds::response::positive_response>
  send_switch_to_level_with_xcp_algorithm(uint32_t tx, uint8_t level);

  /**
   * @brief Request 0x27, Subfunction N/A
   * Requests the server to switch to level using given althorithm for calculating key from seed
   *
   * @param tx address of the server
   * @param level wanted level
   * @param seed_to_key_algorithm algorithm for calculating key from seed
   * @throw uds::exceptions::internal_uds_error_exception if conversion of levels doesn't happen properly (internally in
   * the server)
   * @return response indicating successful request, specific error or negative response otherwise
   */
  DLL_PUBLIC uds::response::uds_client_response<uds::response::positive_response> send_switch_to_level(
      uint32_t tx, uint8_t level,
      std::function<std::vector<uint8_t>(const std::vector<uint8_t>&, int)>& seed_to_key_algorithm);

  /**
   * @brief Gets the active security level of wanted server. Note that this is not an UDS request, but rather the last
   * level the client has requested the server to switch to
   *
   * @param tx address of the server
   * @warning This is not an UDS request, but rather the last level the client has requested the server to switch to
   * @return response containing last level if requested TX is supported, error otherwise
   */
  DLL_PUBLIC uds::response::uds_client_response<int> get_active_level(uint32_t tx);

#ifdef _WIN32
  /**
   * @brief Request 0x27, Subfunction N/A
   * Requests the server to switch to level using dll function for calculating key from seed
   *
   * @param tx address of the server
   * @param level wanted level
   * @param dll_path path to dll from which seed and key algorithm will be read
   * @throw uds::exceptions::internal_uds_error_exception if conversion of levels doesn't happen properly (internally in
   * the server)
   * @return response indicating successful request, specific error or negative response otherwise
   */
  DLL_PUBLIC uds::response::uds_client_response<uds::response::positive_response> send_switch_to_level_with_dll(
      uint32_t tx, uint8_t level, const std::string& dll_path);
#endif

 private:
  u32_uds_client_provider_handle* uds_client_handle;
};
}  // namespace uds_client