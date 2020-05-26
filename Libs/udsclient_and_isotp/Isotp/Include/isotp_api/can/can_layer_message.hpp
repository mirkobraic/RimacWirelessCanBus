#pragma once
#include <cstdint>  // had to add this
#include <vector>

namespace isotp {
/**
 * @brief Represents a CAN message received or sent by the ISO-TP layer
 *
 */
struct can_layer_message {
  std::vector<uint8_t> data;
  uint32_t id;
};
}  // namespace isotp
