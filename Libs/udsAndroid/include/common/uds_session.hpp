#pragma once
#include <string>
namespace uds {

enum class session {
  default_session,
  safety_system_diagnostic_session,
  extended_diagnostic_session,
  programming_session,
  plant_session
};

std::string get_string_from_session(session s);
}  // namespace uds