#ifndef CANBUSINTERFACEFACTORY_H
#define CANBUSINTERFACEFACTORY_H

#include "KvaserWirelessInterface.h"

enum CanBusProvider {
    kvaser
};

class CanBusInterfaceFactory {
private:
    CanBusInterfaceFactory();
public:
    static CanBusInterface* getInterfaceForProvider(CanBusProvider provider) {
        switch (provider) {
        case kvaser:
            return new KvaserWirelessInterface();
        default:
            return nullptr;
        }
    }
};

#endif // CANBUSINTERFACEFACTORY_H
