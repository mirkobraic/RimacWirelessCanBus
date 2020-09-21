#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QObject>
#include <QSettings>

class SettingsManager : public QObject
{
    Q_OBJECT

public:
    SettingsManager(QObject *parent = nullptr);

    Q_PROPERTY(QString rx READ getRx WRITE setRx)
    Q_PROPERTY(QString tx READ getTx WRITE setTx)
    Q_PROPERTY(bool rawCanOverride READ getRawCanOverride WRITE setRawCanOverride)
    Q_PROPERTY(int defaultProvider READ getDefaultProvider WRITE setDefaultProvider)
    Q_PROPERTY(int defaultBaudrate READ getDefaultBaudrate WRITE setDefaultBaudrate)
    Q_PROPERTY(QString defaultIpAddress READ getDefaultIpAddress WRITE setDefaultIpAddress)
    Q_PROPERTY(QString defaultPort READ getDefaultPort WRITE setDefaultPort)

    QString getRx() const;
    void setRx(const QString &value);

    QString getTx() const;
    void setTx(const QString &value);

    bool getRawCanOverride() const;
    void setRawCanOverride(bool value);

    int getDefaultProvider() const;
    void setDefaultProvider(int value);

    int getDefaultBaudrate() const;
    void setDefaultBaudrate(int value);

    QString getDefaultIpAddress();
    void setDefaultIpAddress(const QString &value);

    QString getDefaultPort();
    void setDefaultPort(const QString &value);

private:
    QSettings settings = QSettings("Rimac", "WirelessCanBus");

    QString rx;
    QString tx;
    bool rawCanOverride;
    int defaultProvider;
    int defaultBaudrate;
    QString defaultIpAddress;
    QString defaultPort;
};

#endif // SETTINGSMANAGER_H
