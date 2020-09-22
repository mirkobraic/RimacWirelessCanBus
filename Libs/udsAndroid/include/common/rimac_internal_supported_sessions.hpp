#pragma once
#include <cstdint>

namespace uds {
namespace ra {
enum rimac_internal_supported_sessions : uint8_t {
  default_session = 0x01,
  programming_session = 0x02,
  extended_diagnostic_session = 0x03,
  safety_system_diagnostic_session = 0x04,
  plant_session = 0x40
};
}  // namespace ra
}  // namespace uds
