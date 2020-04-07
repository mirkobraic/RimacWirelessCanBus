#ifndef CANBUSINTERFACEFACTORY_H
#define CANBUSINTERFACEFACTORY_H

#include "KvaserWirelessCan.h"

enum CanBusProvider {
    kvaser
};

class CanBusInterfaceFactory
{
public:
    static CanBusInterface* getInterfaceForProvider(CanBusProvider provider) {
        switch (provider) {
        case kvaser:
            return new KvaserWirelessCan();
        default:
            return nullptr;
        }
    }

private:
    CanBusInterfaceFactory();
};

#endif // CANBUSINTERFACEFACTORY_H
