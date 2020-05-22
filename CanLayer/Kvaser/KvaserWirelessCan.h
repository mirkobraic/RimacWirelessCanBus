#ifndef KVASERWIRELESSCAN_H
#define KVASERWIRELESSCAN_H

#include <QtConcurrent>
#include <QtNetwork>
#include "../CanBusInterface.h"
#include "KvaserNetworkService.h"
#include "canlib.h"

class KvaserWirelessCan : public CanBusInterface
{
    Q_OBJECT

public:
    KvaserWirelessCan();
    ~KvaserWirelessCan();

    void connectToDevice(QString deviceIpAddress, BaudRate baudRate) override;
    void disconnectFromDevice() override;
    void sendCanMessage(isotp::can_layer_message &message) override;

private:
    void startRxThread();
    void canRead(int handle);
    int getBaudRate(BaudRate baudRate);

//    canHandle txHandle = canINVALID_HANDLE;
//    canStatus txStatus = canStatus(0);
    int channelNumber = 0;

    bool isConnected = false;
    bool shouldListen = true;

    /******/
    enum KvaserStatus {
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

//    enum KvaserFlags {
//        canMSG_STD = 2,
//        canMSG_EXT = 4
//    };

//    void initializeLibrary(std::function<void()> callback);
//    void openChannel(int *txHandle, std::function<void()> callback);
//    void setBusParams(int handle, int baudRate, std::function<void()> callback);
//    void canBusOn(int handle, std::function<void()> callback);
//    void canBusOff(int handle, std::function<void()> callback);
//    void closeChannel(int handle, std::function<void()> callback);
//    void unloadLibrary(std::function<void()> callback);

//    void checkStatus(QString method, KvaserStatus status);

    bool checkStatus(QString method, KvaserNetworkService::Status status);

    KvaserNetworkService kvNetService;
    KvaserNetworkService *rxKvNetService;
    QString deviceIpAddress;
    QString port;

    QNetworkAccessManager networkManager;
    QUrl baseUrl;
    QString sessionId;
    int txHandle;
};

#endif // KVASERWIRELESSCAN_H
