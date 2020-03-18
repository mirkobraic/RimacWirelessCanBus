#ifndef CANBUSINTERFACE_H
#define CANBUSINTERFACE_H

#include <QObject>
#include "CanMessage.h"

class CanBusInterface : public QObject {
public:
    virtual ~CanBusInterface() {}

    virtual void connect() = 0;
    virtual void disconnect() = 0;
    virtual void sendCanMessage(CanMessage message) = 0;

signals:
    virtual void newDataFrame(CanMessage message) = 0;

    // QUESTION: why is this not working
//    static CanBusInterface* getBusInterfaceForProvider(CanBusProvider provider) {
//        switch (provider) {
//        case kvaser:
//            return new KvaserWirelessInterface();
//        default:
//            return nullptr;
//        }
//    }
};

#endif // CANBUSINTERFACE_H
