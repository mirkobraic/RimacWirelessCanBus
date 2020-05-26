#pragma once
#include <stdexcept>

namespace uds {
namespace exceptions {
class missing_uds_environment_variable_exception : public std::logic_error {
 public:
  missing_uds_environment_variable_exception(const char* message, const char* missing_variable)
      : std::logic_error{message}, missing_variable{missing_variable} {}

  const char* get_missing_variable() { return missing_variable; };

 private:
  const char* missing_variable;
};
}  // namespace exceptions
}  // namespace uds
