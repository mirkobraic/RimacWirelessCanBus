#include "CommunicationManager.h"

CommunicationManager::CommunicationManager(CanBusProvider provider, std::pair<uint32_t, uint32_t> rxTxPair, QObject *parent)
    : QObject(parent)
{
    this->rxTxPair = rxTxPair;
    logger = std::make_shared<Logger>();
    canBusInterface = CanBusInterfaceFactory::getInterfaceForProvider(provider);
    QObject::connect(canBusInterface.get(), SIGNAL(showAlert(QString, QString)), this, SLOT(onShowAlert(QString, QString)));
    QObject::connect(canBusInterface.get(), SIGNAL(fetchingInProgress(bool)), this, SLOT(onFetchingInProgress(bool)));
    QObject::connect(canBusInterface.get(), SIGNAL(newDirectCanMessage(uint32_t, std::vector<uint8_t>)), this, SLOT(onNewDirectCanMessage(uint32_t, std::vector<uint8_t>)));
    QObject::connect(canBusInterface.get(), SIGNAL(toggleConnection(bool)), this, SLOT(onToggleConnection(bool)));

    isotpTransport = IsotpManager::makeTransportLayer(canBusInterface, rxTxPair, logger);

    udsClient = UdsManager::makeUdsClient(isotpTransport, rxTxPair, logger);

    timer.start();
}

void CommunicationManager::connect(QString ipAddress, QString port, BaudRate baudRate)
{
    canBusInterface->connectToDevice(ipAddress, port, baudRate);
}

void CommunicationManager::disconnect()
{
    canBusInterface->disconnectFromDevice();
}

void CommunicationManager::sendDirectCanMessage(std::vector<uint8_t> data, uint32_t id)
{
    isotp::can_layer_message msg;
    msg.data = data;
    msg.id = id;
    canBusInterface->sendCanMessage(msg);
}

void CommunicationManager::udsCheckVersion()
{
    emit fetchingInProgress(true);
    auto positiveResponse = [this] (const std::pair<uds::version_params, uds::type_of_server> &pair) {
        QString message = "Server version: " + QString::number(pair.first.major) + "." + QString::number(pair.first.minor) + "." + QString::number(pair.first.patch);
        QString serverType = pair.second == uds::dynamic_library ? "Dynamic library" : "Embedded code";
        message += "\n\nType of server: " + serverType;
        emit fetchingInProgress(false);
        emit showAlert("Success", message);
    };

    uint32_t tx = rxTxPair.second;
    auto response = udsClient->check_version_servicees.send_check_version(tx);
    response.unpack_response(positiveResponse, negativeResponse, errorResponse);
}

void CommunicationManager::udsGetSupportedDtcsStatus(std::function<void (const std::vector<int>&, const std::vector<int>&)> callback)
{
    emit fetchingInProgress(true);
    auto positiveResponse = [this, callback] (const std::map<dtc_mask, dtc_status> &res) {
        emit fetchingInProgress(false);
        std::vector<int> masks;
        std::vector<int> statuses;

        for (auto const& [key, val] : res) {
            masks.push_back(key);
            statuses.push_back(val);
        }
        callback(masks, statuses);
    };

    uint32_t tx = rxTxPair.second;
    auto response = udsClient->dtc_api_services.get_status_of_all_supported_dtcs(tx);
    response.unpack_response(positiveResponse, negativeResponse, errorResponse);
}

void CommunicationManager::udsClearDtcInformation()
{
    emit fetchingInProgress(true);
    auto positiveResponse = [this] (const uds::response::positive_response) {
        emit fetchingInProgress(false);
        emit showAlert("Success", "DTC information has been successfully cleared.");
    };

    uint32_t tx = rxTxPair.second;
    auto response = udsClient->dtc_api_services.send_clear_dtc_information(tx, 0xFFFFFF);
    response.unpack_response(positiveResponse, negativeResponse, errorResponse);
}

void CommunicationManager::onNewDirectCanMessage(uint32_t id, std::vector<uint8_t> data)
{
    try {
        CanMessage canMessage = CanMessage(id, data.size(), data);
        canMessage.setTimestamp(timer.elapsed());
        emit newCanMessageRecieved(canMessage);
    } catch (std::exception e) {
        qDebug() << "CanMessage constructor error:  " << e.what();
    }
}

void CommunicationManager::onShowAlert(QString title, QString message)
{
    emit showAlert(title, message);
}

void CommunicationManager::onFetchingInProgress(bool value)
{
    emit fetchingInProgress(value);
}

void CommunicationManager::onToggleConnection(bool value)
{
    emit toggleConnection(value);
}
