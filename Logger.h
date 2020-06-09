#ifndef LOGGER_H
#define LOGGER_H

#include <QDebug>
#include "isotp_api/logger.hpp"

class Logger : public logger
{
public:
    Logger();

    void log_debug(const std::string& message) override;
    void log_info(const std::string& message) override;
    void log_warn(const std::string& message) override;
    void log_error(const std::string& message) override;
    bool is_debug_active() override;
};

#endif // LOGGER_H
