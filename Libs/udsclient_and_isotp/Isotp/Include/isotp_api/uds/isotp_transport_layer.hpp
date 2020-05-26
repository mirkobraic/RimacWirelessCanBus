#pragma once
#include <memory>
#include <utility>
#include "isotp_api/can/isotp_can_provider.hpp"
#include "isotp_api/export_macro.hpp"
#include "isotp_api/logger.hpp"
#include "isotp_api/uds/transport_layer.hpp"

namespace isotp {

struct isotp_configuration {
  uint8_t isotp_state_machines_thread_pool_size;
  uint8_t isotp_received_messages_callback_thread_pool_size;
};

class isotp_fsm_manager;  // forward declare the implementation class so that no additional headers get included here
/**
 * @brief ISO-TP provider that implements the transport layer interface from the UDS project
 * Overriden methods are explained in the UDS transport layer doxygen
 *
 */
class isotp_transport_layer final : public uds::transport_layer<uint32_t> {
 public:
  /**
   * @brief Construct a new isotp transport layer object
   *
   * @param can A can provider that will be used by the ISO-TP layer
   * @param rx_tx_pairs List of pairs for which the ISO-TP manager will create state machines.
   */
  DLL_PUBLIC isotp_transport_layer(std::shared_ptr<can_provider> can,
                                   const std::vector<std::pair<uint32_t, uint32_t>>& rx_tx_pairs,
                                   std::shared_ptr<logger> logger);

  DLL_PUBLIC isotp_transport_layer(std::shared_ptr<can_provider> can,
                                   const std::vector<std::pair<uint32_t, uint32_t>>& rx_tx_pairs,
                                   const isotp_configuration& thread_pool_configuration,
                                   std::shared_ptr<logger> logger);

  DLL_PUBLIC virtual ~isotp_transport_layer();
  DLL_PUBLIC void send_message_to_peer(std::unique_ptr<uds::uds_message<uint32_t>> message) override;
  DLL_PUBLIC void set_received_message_callback(
      std::function<void(std::shared_ptr<uds::uds_message<uint32_t>>)> callback) override;

 private:
  isotp_fsm_manager* manager;
  std::shared_ptr<logger> log;
};
}  // namespace isotp