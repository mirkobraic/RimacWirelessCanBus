#ifndef KVASERNETWORKSERVICE_H
#define KVASERNETWORKSERVICE_H

#include <QObject>
#include <QtNetwork>

class KvaserNetworkService : public QObject
{
    Q_OBJECT
public:
    explicit KvaserNetworkService(QObject *parent = nullptr);

    enum Status {
        canOK = 0,
        canERR_PARAM = -1,
        canERR_NOMSG = -2,
        canERR_NOMEM = -3,
        canERR_NOCHANNELS = -5,
        canERR_TIMEOUT = -7,
        canERR_INVHANDLE = -10,
        canERR_HARDWARE = -15,
        canERR_NOT_IMPLEMENTED = -32,
        canERR_DEVICE_FILE = -33,
        canERR_SCRIPT_FAIL = -39,
        canERR_SCRIPT_WRONG_VERSION = -40,
        canERR_INVALID_PASSWORD = -128,
        canERR_NO_SUCH_FUNCTION = -129,
        canERR_NOT_AUTHORIZED = -130,
        canERR_INVALID_SESSION = -131
    };

    void setupDevice(QString ipAddress, QString port);

    void initializeLibrary(std::function<void (Status status, QString sessionId)> callback);
    void openChannel(QString sessionId, std::function<void (Status status, int handle)> callback);
    void setBusParams(QString sessionId, int handle, int baudRate, std::function<void (Status status)> callback);
    void canBusOn(QString sessionId, int handle, std::function<void (Status status)> callback);
    void canWrite(QString sessionId, int handle, uint32_t id, uint flag, QString data, size_t dlc, std::function<void (Status status)> callback);
    void canRead(QString sessionId, int handle, std::function<void (Status status, uint32_t id, std::vector<uint8_t> data)> callback);
    void canBusOff(QString sessionId, int handle, std::function<void (Status status)> callback);
    void closeChannel(QString sessionId, int handle, std::function<void (Status status)> callback);
    void unloadLibrary(QString sessionId, std::function<void (Status status)> callback);

private:
    QNetworkAccessManager networkManager;

    QUrl baseUrl;

};

#endif // KVASERNETWORKSERVICE_H
