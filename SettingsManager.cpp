#include "SettingsManager.h"

SettingsManager::SettingsManager(QObject *parent) : QObject(parent)
{
    rx = settings.value("rx").toString();
    tx = settings.value("tx").toString();
    rawCanOverride = settings.value("setRawCanOverride").toBool();
    defaultProvider = settings.value("defaultProvider").toInt();
    defaultBaudrate = settings.value("defaultBaudrate").toInt();
    defaultIpAddress = settings.value("defaultIpAddress" + QString::number(defaultProvider)).toString();
    defaultPort = settings.value("defaultPort" + QString::number(defaultProvider)).toString();
}

QString SettingsManager::getRx() const
{
    return rx;
}

void SettingsManager::setRx(const QString &value)
{
    settings.setValue("rx", value);
    rx = value;
}

QString SettingsManager::getTx() const
{
    return tx;
}

void SettingsManager::setTx(const QString &value)
{
    settings.setValue("tx", value);
    tx = value;
}

bool SettingsManager::getRawCanOverride() const
{
    return rawCanOverride;
}

void SettingsManager::setRawCanOverride(bool value)
{
    settings.setValue("setRawCanOverride", value);
    rawCanOverride = value;
}

int SettingsManager::getDefaultProvider() const
{
    return defaultProvider;
}

void SettingsManager::setDefaultProvider(int value)
{
    settings.setValue("defaultProvider", value);
    defaultProvider = value;
}

int SettingsManager::getDefaultBaudrate() const
{
    return defaultBaudrate;
}

void SettingsManager::setDefaultBaudrate(int value)
{
    settings.setValue("defaultBaudrate", value);
    defaultBaudrate = value;
}

QString SettingsManager::getDefaultIpAddress()
{
    defaultIpAddress = settings.value("defaultIpAddress" + QString::number(defaultProvider)).toString();
    return defaultIpAddress;
}

void SettingsManager::setDefaultIpAddress(const QString &value)
{
    settings.setValue("defaultIpAddress" + QString::number(defaultProvider), value);
    defaultIpAddress = value;
}

QString SettingsManager::getDefaultPort()
{
    defaultPort = settings.value("defaultPort" + QString::number(defaultProvider)).toString();
    return defaultPort;
}

void SettingsManager::setDefaultPort(const QString &value)
{
    settings.setValue("defaultPort" + QString::number(defaultProvider), value);
    defaultPort = value;
}
