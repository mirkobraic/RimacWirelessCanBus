#ifndef UDSLOGGER_H
#define UDSLOGGER_H
#include <string>

class logger {
 public:
  virtual ~logger() = default;
  virtual void log_debug(const std::string& message) = 0;
  virtual void log_info(const std::string& message) = 0;
  virtual void log_warn(const std::string& message) = 0;
  virtual void log_error(const std::string& message) = 0;
  virtual bool is_debug_active() = 0;
};

#endif