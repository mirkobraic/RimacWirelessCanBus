#pragma once
#include <functional>
#include <memory>
#include "isotp_api/can/can_layer_message.hpp"

namespace isotp {
/**
 * @brief Can provider interface that will be used by the ISO-TP layer to send and receive messages over CAN
 *
 */
class can_provider {
 public:
  virtual ~can_provider() = default;
  /**
   * @brief This will be called by the ISO-TP layer to send CAN messages
   *
   * @param message what to send
   */
  virtual void send_can_message(std::unique_ptr<isotp::can_layer_message> message) = 0;

  /**
   * @brief Sets the callback on any received CAN messages of interest
   *
   */
  virtual void set_received_message_callback(std::function<void(std::unique_ptr<isotp::can_layer_message>)>) = 0;
};
}  // namespace isotp
