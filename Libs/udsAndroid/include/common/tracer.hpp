#pragma once
#include <vector>

namespace uds {
namespace trace {
template <typename T>
struct trace_message_info {
  T tx;
  T rx;
  uint8_t request;
  uint8_t subfunction;
  std::vector<uint8_t> message_data;
};

template <typename T>
class tracer {
 public:
  virtual ~tracer() = default;
  virtual void on_message_received(trace_message_info<T> message) = 0;
  virtual void on_message_sent(trace_message_info<T> message) = 0;
};
}  // namespace trace
}  // namespace uds
