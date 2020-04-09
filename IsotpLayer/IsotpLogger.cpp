#include "IsotpLogger.h"

IsotpLogger::IsotpLogger()
{ }

void IsotpLogger::log_debug(const std::string &message)
{
    qDebug() << "Debug: " + QString::fromStdString(message);
}

void IsotpLogger::log_info(const std::string &message)
{
    qDebug() << "Info: " + QString::fromStdString(message);
}

void IsotpLogger::log_warn(const std::string &message)
{
    qDebug() << "Warning: " + QString::fromStdString(message);
}

void IsotpLogger::log_error(const std::string &message)
{
    qDebug() << "Error: " + QString::fromStdString(message);
}

bool IsotpLogger::is_debug_active()
{
    return true;
}
