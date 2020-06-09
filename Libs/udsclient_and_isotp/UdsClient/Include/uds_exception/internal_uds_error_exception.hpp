#pragma once
#include <stdexcept>

namespace uds {
namespace exceptions {
class internal_uds_error_exception : public std::logic_error {
 public:
  internal_uds_error_exception(const char* message) : std::logic_error{message} {}
};
}  // namespace exceptions
}  // namespace uds
