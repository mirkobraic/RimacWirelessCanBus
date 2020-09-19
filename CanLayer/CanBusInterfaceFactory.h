#ifndef CANBUSINTERFACEFACTORY_H
#define CANBUSINTERFACEFACTORY_H

#include "Kvaser/KvaserWirelessCan.h"
#include "Wiicom/WiicomWirelessCan.h"

enum CanBusProvider {
    // raw value is connected with qml
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
        case wiicom:
            return std::make_shared<WiicomWirelessCan>();
        }
    }

private:
    CanBusInterfaceFactory();
};

#endif // CANBUSINTERFACEFACTORY_H
