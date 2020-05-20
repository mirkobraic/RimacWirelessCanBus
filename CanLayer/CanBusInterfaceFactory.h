#ifndef CANBUSINTERFACEFACTORY_H
#define CANBUSINTERFACEFACTORY_H

#include "KvaserWirelessCan.h"

enum CanBusProvider {
    kvaser = 0,
    wiicom = 1
};

class CanBusInterfaceFactory
{
public:
    static std::shared_ptr<CanBusInterface> getInterfaceForProvider(CanBusProvider provider) {
        switch (provider) {
        case kvaser:
            return std::make_shared<KvaserWirelessCan>();
        default:
            return nullptr;
        }
    }

private:
    CanBusInterfaceFactory();
};

#endif // CANBUSINTERFACEFACTORY_H
