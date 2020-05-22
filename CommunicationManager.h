#ifndef COMMUNICATIONMANAGER_H
#define COMMUNICATIONMANAGER_H

#include <QObject>
#include "CanLayer/CanBusInterfaceFactory.h"
#include "IsotpLayer/IsotpManager.h"
#include "UdsLayer/UdsConfigManager.h"
#include "UdsLayer/UdsManager.h"
#include "Models/CanMessage.h"
#include "uds_client_api/client_services_structures.hpp"

class CommunicationManager : public QObject
{
    Q_OBJECT

public:
    CommunicationManager(CanBusProvider provider, std::vector<std::pair<uint32_t, uint32_t>> rxTxPairs, QObject *parent = nullptr);

    void connect(QString ipAddress, BaudRate baudRate);
    void disconnect();

    void sendDirectCanMessage(std::vector<uint8_t> data, uint32_t id);

    void udsCheckVersion(uint32_t tx, std::function<void(QString, QString)> callback);

signals:
    void newCanMessageRecieved(CanMessage message);

private:
    std::vector<std::pair<uint32_t, uint32_t>> rxTxPairs;
    std::shared_ptr<Logger> logger;

    std::shared_ptr<CanBusInterface> canBusInterface;

    std::shared_ptr<isotp::isotp_transport_layer> isotpTransport;

    std::unique_ptr<uds_client::uds_client_with_u32_ids> udsClient;
};

#endif // COMMUNICATIONMANAGER_H
