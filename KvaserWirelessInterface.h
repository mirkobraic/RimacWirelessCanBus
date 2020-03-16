#ifndef KVASERWIRELESSINTERFACE_H
#define KVASERWIRELESSINTERFACE_H

#include "CanBusInterface.h"
#include "kvrlib.h"
#include "canlib.h"

class KvaserWirelessInterface : public CanBusInterface
{
public:
    KvaserWirelessInterface();

    void connect() override;
    void disconnect() override;
    void sendCanMessage() override;
    void recievedDataFrame() override;
};

#endif // KVASERWIRELESSINTERFACE_H
