#pragma once

#include <cstdint>
#include <vector>

struct isotp_message {
  std::vector<uint8_t> data;
  uint32_t id;
};
