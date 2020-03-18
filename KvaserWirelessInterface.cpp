#include "KvaserWirelessInterface.h"

KvaserWirelessInterface::KvaserWirelessInterface()
{

}

void KvaserWirelessInterface::connect()
{

}

void KvaserWirelessInterface::disconnect()
{

}

void KvaserWirelessInterface::sendCanMessage(CanMessage message)
{
    emit newDataFrame(message);
}
