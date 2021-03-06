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

    void connectToDevice(QString deviceIpAddress, QString port, BaudRate baudRate) override;
    void disconnectFromDevice() override;
    void sendCanMessage(isotp::can_layer_message &message) override;

private:
    void readMessage();
    bool checkStatus(QString method, KvaserResponse res);

    KvaserNetworkService kvNetService;
    QTimer *rxTimer;

    QString sessionId;
    int handle;

    int channelNumber = 0;
    bool isConnected = false;
};

#endif // KVASERWIRELESSCAN_H
