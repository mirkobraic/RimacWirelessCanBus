#pragma once
#include <vector>
extern "C" {
#include "common/xcp_seednkey.h"
}
namespace uds {
std::vector<uint8_t> generate_key_from_seed_xcp_algorithm(const std::vector<uint8_t>&, uint8_t);
}