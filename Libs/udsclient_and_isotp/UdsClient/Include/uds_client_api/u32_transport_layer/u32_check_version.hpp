#pragma once

#include "uds_client_api/client_services_structures.hpp"
#include "uds_client_api/export_macro.hpp"
#include "uds_client_api/u32_transport_layer/u32_uds_client_provider_handle.hpp"
#include "uds_client_api/uds_return_types.hpp"

namespace uds_client {
class u32_check_version final {
 public:
  explicit u32_check_version(u32_uds_client_provider_handle* handle);

  /**
   * @brief Sends check version request (this service is not a part of UDS standard, it is only supported on Rimac
   * server implementations (C++ and embedded))
   *
   * @param tx address of the server
   * @return response containing information about server (server type (embedded or dynamic library) and server version)
   * if request was successful, specific error or negative response otherwise
   */
  DLL_PUBLIC uds::response::uds_client_response<std::pair<uds::version_params, uds::type_of_server>> send_check_version(
      uint32_t tx);

 private:
  u32_uds_client_provider_handle* uds_client_handle;
};
}  // namespace uds_client