#include "Logger.h"

Logger::Logger()
{ }

void Logger::log_debug(const std::string &message)
{
    qDebug() << "Debug: " + QString::fromStdString(message);
}

void Logger::log_info(const std::string &message)
{
    qDebug() << "Info: " + QString::fromStdString(message);
}

void Logger::log_warn(const std::string &message)
{
    qDebug() << "Warning: " + QString::fromStdString(message);
}

void Logger::log_error(const std::string &message)
{
    qDebug() << "Error: " + QString::fromStdString(message);
}

bool Logger::is_debug_active()
{
    return true;
}
