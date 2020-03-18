#ifndef KVASERWIRELESSINTERFACE_H
#define KVASERWIRELESSINTERFACE_H

#include "CanBusInterface.h"
#include "kvrlib.h"
#include "canlib.h"

class KvaserWirelessInterface : public CanBusInterface {
    Q_OBJECT
public:
    KvaserWirelessInterface();

    void connect() override;
    void disconnect() override;
    void sendCanMessage(CanMessage message) override;

signals:
    void newDataFrame(CanMessage message) override;
};

#endif // KVASERWIRELESSINTERFACE_H
