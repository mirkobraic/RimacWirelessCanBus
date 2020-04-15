#ifndef KVASERWIRELESSINTERFACE_H
#define KVASERWIRELESSINTERFACE_H

#include <QtConcurrent>
#include "CanBusInterface.h"
#include "canlib.h"

class KvaserWirelessInterface : public CanBusInterface
{
    Q_OBJECT

public:
    KvaserWirelessInterface();
    ~KvaserWirelessInterface();

    void connect(QString channelName, BaudRate baudRate) override;
    void disconnect() override;
    void sendCanMessage(CanMessage message) override;

signals:
    void newDataFrame(CanMessage message) override;

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

#endif // KVASERWIRELESSINTERFACE_H
