#ifndef CANBUSMANAGER_H
#define CANBUSMANAGER_H

#include <QObject>
#include "CanBusInterfaceFactory.h"

class CanBusManager : public QObject
{
    Q_OBJECT
public:
    CanBusManager(CanBusProvider);

private:
    CanBusInterface *canBusInterface = nullptr;
};

#endif // CANBUSMANAGER_H
