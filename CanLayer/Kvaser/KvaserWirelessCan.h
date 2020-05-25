#ifndef KVASERWIRELESSCAN_H
#define KVASERWIRELESSCAN_H

#include "../CanBusInterface.h"
#include "KvaserNetworkService.h"

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
    void readMessage();
    bool checkStatus(QString method, ResponseStatus status);

    KvaserNetworkService kvNetService;
    QTimer *rxTimer;

    QString deviceIpAddress;
    QString port;

    QString sessionId;
    int handle;

    int channelNumber = 0;
    bool isConnected = false;
};

#endif // KVASERWIRELESSCAN_H
