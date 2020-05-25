#ifndef KVASERNETWORKSERVICE_H
#define KVASERNETWORKSERVICE_H

#include <QObject>
#include <QtNetwork>
#include "../CanBusInterface.h"
#include "KvaserConstants.h"

using namespace KvaserConstants;

class KvaserNetworkService : public QObject
{
    Q_OBJECT
public:
    explicit KvaserNetworkService(QObject *parent = nullptr);

    void setupDevice(QString ipAddress, QString port);
    int getKvaserBaudRate(BaudRate baudRate);

    void initializeLibrary(std::function<void (ResponseStatus status, QString sessionId)> callback);
    void openChannel(QString sessionId, int channel, OpenFlags flags, std::function<void (ResponseStatus status, int handle)> callback);
    void setBaudRate(QString sessionId, int handle, BaudRate baudRate, std::function<void (ResponseStatus status)> callback);
    void canBusOn(QString sessionId, int handle, std::function<void (ResponseStatus status)> callback);
    void canWrite(QString sessionId, int handle, uint32_t id, uint flag, QString data, size_t dlc, std::function<void (ResponseStatus status)> callback);
    void canRead(QString sessionId, int handle, int max, std::function<void (ResponseStatus status, uint flag, uint32_t id, std::vector<uint8_t> data)> callback);
    void canBusOff(QString sessionId, int handle, std::function<void (ResponseStatus status)> callback);
    void closeChannel(QString sessionId, int handle, std::function<void (ResponseStatus status)> callback);
    void unloadLibrary(QString sessionId, std::function<void (ResponseStatus status)> callback);

private:
    QNetworkAccessManager networkManager;
    QUrl baseUrl;
};

#endif // KVASERNETWORKSERVICE_H
