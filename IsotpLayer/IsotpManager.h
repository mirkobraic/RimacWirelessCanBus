#ifndef ISOTPMANAGER_H
#define ISOTPMANAGER_H

#include <QObject>
#include "isotp_api/uds/isotp_transport_layer.hpp"
#include "CanLayer/CanBusInterfaceFactory.h"
#include "IsotpLayer/IsotpCanProvider.h"
#include "IsotpLayer/IsotpLogger.h"
#include "Models/CanMessage.h"

class IsotpManager : public QObject
{
    Q_OBJECT

public:
    IsotpManager(CanBusProvider provider, std::vector<std::pair<uint32_t, uint32_t>> rxTxPairs, QObject *parent = nullptr);

    void connect(std::string name, BaudRate baudRate);
    void disconnect();
    void sendMessage(uds::uds_message<uint32_t> message);

signals:
    void newMessageRecieved(CanMessage message);

private:
    std::shared_ptr<CanBusInterface> canBusInterface;

    std::shared_ptr<IsotpCanProvider> isotpCanProvider;
    std::shared_ptr<IsotpLogger> isotpLogger;

    isotp::isotp_transport_layer isotpTransport;

};

#endif // ISOTPMANAGER_H
