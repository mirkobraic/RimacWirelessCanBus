#ifndef COMMUNICATIONMANAGER_H
#define COMMUNICATIONMANAGER_H

#include <QObject>

#include "CanLayer/CanBusInterfaceFactory.h"
#include "Models/CanMessage.h"

#include "IsotpLayer/IsotpManager.h"

#include "UdsLayer/UdsConfigManager.h"
#include "UdsLayer/UdsManager.h"
#include "UdsLayer/UdsConstantsUnpacker.h"
#include "uds_client_api/client_services_structures.hpp"

class CommunicationManager : public QObject
{
    Q_OBJECT

public:
    CommunicationManager(CanBusProvider provider, std::pair<uint32_t, uint32_t> rxTxPair, QObject *parent = nullptr);

    void connect(QString ipAddress, QString port, BaudRate baudRate);
    void disconnect();

    void sendDirectCanMessage(std::vector<uint8_t> data, uint32_t id);

    void udsCheckVersion();
    void udsGetSupportedDtcsStatus(std::function<void (const std::vector<int>&, const std::vector<int>&)>);
    void udsClearDtcInformation();

signals:
    void newCanMessageRecieved(CanMessage message);
    void showAlert(QString title, QString message);
    void fetchingInProgress(bool value);
    void toggleConnection(bool value);

public slots:
    void onNewDirectCanMessage(uint32_t id, std::vector<uint8_t> data);
    void onShowAlert(QString title, QString message);
    void onFetchingInProgress(bool value);
    void onToggleConnection(bool value);

private:
    std::pair<uint32_t, uint32_t> rxTxPair;
    std::shared_ptr<Logger> logger;

    std::shared_ptr<CanBusInterface> canBusInterface;

    std::shared_ptr<isotp::isotp_transport_layer> isotpTransport;

    std::unique_ptr<uds_client::uds_client_with_u32_ids> udsClient;

    QElapsedTimer timer;

    std::function<void(const uds::response::negative_response&)> negativeResponse = [this] (const uds::response::negative_response &res) {
        QString message = UdsConstantsUnpacker::unpackNegativeResponse(res) + "\ncode: " + QString::number((int)res);
        emit fetchingInProgress(false);
        emit showAlert("Negative response", message);
    };
    std::function<void(const uds::response::response_error&)> errorResponse = [this] (const uds::response::response_error &res) {
        QString message = UdsConstantsUnpacker::unpackResponseError(res) + "\ncode: " + QString::number((int)res);
        emit fetchingInProgress(false);
        emit showAlert("Error", message);
    };

};

#endif // COMMUNICATIONMANAGER_H
