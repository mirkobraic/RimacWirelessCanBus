#pragma once
#include "common/logger.hpp"
#include "common/tracer.hpp"
#include "common/transport_layer.hpp"
#include "uds_client_api/client_configuration_manager.hpp"
#include "uds_client_api/client_services_structures.hpp"
#include "uds_client_api/export_macro.hpp"
#include "uds_client_api/u32_transport_layer/u32_access_timing_parameter.hpp"
#include "uds_client_api/u32_transport_layer/u32_check_version.hpp"
#include "uds_client_api/u32_transport_layer/u32_communication_control.hpp"
#include "uds_client_api/u32_transport_layer/u32_data_transfer.hpp"
#include "uds_client_api/u32_transport_layer/u32_dtc_api.hpp"
#include "uds_client_api/u32_transport_layer/u32_ecu_reset.hpp"
#include "uds_client_api/u32_transport_layer/u32_programming.hpp"
#include "uds_client_api/u32_transport_layer/u32_routine_control.hpp"
#include "uds_client_api/u32_transport_layer/u32_security_levels_and_sessions.hpp"
#include "uds_client_api/u32_transport_layer/u32_uds_client_provider_handle.hpp"
#include "uds_client_api/uds_return_types.hpp"

namespace uds_client {
class uds_client_with_u32_ids {
 public:
  /**
   * @brief Constructor for uds client with u32 ids without tracing
   *
   * @param transport transport layer which uses u32_t as a descriptor of communication endpoints (for example, CAN TX
   * and RX)
   * @param configuration_manager configuration provider which uses u32_t as a descriptor of communication endpoints
   * (for example, CAN TX and RX)
   * @param logger logging provider used in the client
   */
  DLL_PUBLIC uds_client_with_u32_ids(
      std::shared_ptr<uds::transport_layer<uint32_t>> transport,
      std::unique_ptr<uds::client::configuration::client_configuration_manager<uint32_t>> configuration_manager,
      std::shared_ptr<logger> logger);

  /**
   * @brief Constructor for uds client with u32 ids with tracing
   *
   * @param transport transport layer which uses u32_t as a descriptor of communication endpoints (for example, CAN TX
   * and RX)
   * @param configuration_manager configuration provider which uses u32_t as a descriptor of communication endpoints
   * (for example, CAN TX and RX)
   * @param logger logging provider used in the client
   * @param tracer tracing provider which will give information about all sent and received messages
   */
  DLL_PUBLIC uds_client_with_u32_ids(
      std::shared_ptr<uds::transport_layer<uint32_t>> transport,
      std::unique_ptr<uds::client::configuration::client_configuration_manager<uint32_t>> configuration_manager,
      std::shared_ptr<logger> logger, std::shared_ptr<uds::trace::tracer<uint32_t>> tracer);

  DLL_PUBLIC virtual ~uds_client_with_u32_ids();

 private:
  u32_uds_client_provider_handle* inner;

 public:
  /**
   * @brief member used for sending timing parameter requests
   *
   */
  u32_access_timing_parameter access_timing_parameter_services;

  /**
   * @brief member used for sending check version requests
   *
   */
  u32_check_version check_version_servicees;

  /**
   * @brief member used for sending communication control requests
   *
   */
  u32_communication_control communication_control_services;

  /**
   * @brief member used for sending data transfer requests
   *
   */
  u32_data_transfer data_transfer_services;

  /**
   * @brief member used for sending diagnostics-related requests
   *
   */
  u32_dtc_api dtc_api_services;

  /**
   * @brief member used for sending ecu-reset requests
   *
   */
  u32_ecu_reset ecu_reset_services;

  /**
   * @brief member used for sending sessions and security levels requests
   *
   */
  u32_security_levels_and_sessions security_levels_and_sessions_services;

  /**
   * @brief member used for sending routine control requests
   *
   */
  u32_routine_control routine_control_services;

  /**
   * @brief member used for sending programming requests
   *
   */
  u32_programming programming_services;
};
}  // namespace uds_client
