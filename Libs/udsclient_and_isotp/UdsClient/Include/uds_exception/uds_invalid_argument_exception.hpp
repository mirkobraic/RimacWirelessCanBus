#pragma once
#include <stdexcept>

namespace uds {
namespace exceptions {
class uds_invalid_argument : public std::invalid_argument {
 public:
  uds_invalid_argument(const char* message) : std::invalid_argument{message} {}
};
}  // namespace exceptions
}  // namespace uds
