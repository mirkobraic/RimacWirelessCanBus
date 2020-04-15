#ifndef KVASERWIRELESSCAN_H
#define KVASERWIRELESSCAN_H

#include <QtConcurrent>
#include "CanBusInterface.h"
#include "canlib.h"

class KvaserWirelessCan : public CanBusInterface
{
public:
    KvaserWirelessCan();
    ~KvaserWirelessCan();

    void connect(std::string channelName, BaudRate baudRate) override;
    void disconnect() override;
    void sendCanMessage(isotp::can_layer_message &message) override;

private:
    void startListening();
    int getChannelCount();
    void checkStatus(QString method, canStatus txStatus);
    int getBaudRate(BaudRate baudRate);

    canHandle txHandle = canINVALID_HANDLE;
    canStatus txStatus = canOK;
    int channelNumber = 0;

    bool isConnected = false;
    bool shouldListen = true;
};

#endif // KVASERWIRELESSCAN_H
