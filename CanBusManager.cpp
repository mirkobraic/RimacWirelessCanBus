#include "CanBusManager.h"

CanBusManager::CanBusManager(CanBusProvider provider)
{
    canBusInterface = CanBusInterfaceFactory::getInterfaceForProvider(provider);
}
