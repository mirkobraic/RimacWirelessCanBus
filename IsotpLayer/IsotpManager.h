#ifndef ISOTPMANAGER_H
#define ISOTPMANAGER_H

#include <QObject>
#include "isotp_api/uds/isotp_transport_layer.hpp"
#include "IsotpLayer/IsotpCanProvider.h"
#include "Logger.h"

class IsotpManager
{
public:
    static std::shared_ptr<isotp::isotp_transport_layer> makeTransportLayer(std::shared_ptr<CanBusInterface> canBusInterface,
                                                                            std::pair<uint32_t, uint32_t> rxTxPairs,
                                                                            std::shared_ptr<Logger> logger);

private:
    IsotpManager();

};

#endif // ISOTPMANAGER_H
