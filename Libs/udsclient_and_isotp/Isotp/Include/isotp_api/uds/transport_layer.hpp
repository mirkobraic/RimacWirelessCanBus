#ifndef UDS_TRANSPORT_LAYER_INCLUDE
#define UDS_TRANSPORT_LAYER_INCLUDE
#include <functional>
#include <memory>
#include <vector>

namespace uds {
template <typename T>
struct uds_message {
  T sender_id;
  T recipient_id;
  std::vector<uint8_t> data;
};

/**
 * @brief Interface for transport layer used by the UDS server
 * Any implementation needs to take into account that multiple threads might call this at the same time
 *
 */
template <typename T>
class transport_layer {
 public:
  virtual ~transport_layer() = default;
  /**
   * @brief Method used by the UDS layer when it wants to send a message (usually a response on some client request)
   *
   * @param message message to be sent
   */
  virtual void send_message_to_peer(std::unique_ptr<uds_message<T>> message) = 0;

  /**
   * @brief UDS layer uses this method to set the callback on any received messages of interesty
   *
   * @param callback
   */
  virtual void set_received_message_callback(std::function<void(std::shared_ptr<uds_message<T>>)> callback) = 0;
};
}  // namespace uds

#endif