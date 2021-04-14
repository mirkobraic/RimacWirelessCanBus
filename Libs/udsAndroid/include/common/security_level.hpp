#pragma once
#include <memory>
#include <optional>
#include <string>
namespace uds {
enum class security_level { zero, one, two, three, four };

uint32_t get_const_for_seed_with_level(security_level level);

std::optional<security_level> get_requested_security_level_from_message(uint8_t byte);

uint8_t get_subfunction_from_wanted_level(security_level level);

std::string security_level_as_string(security_level);
}  // namespace uds
