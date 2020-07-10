#ifndef COMMUNICATIONMANAGER_H
#define COMMUNICATIONMANAGER_H

#include <QObject>
#include "CanLayer/CanBusInterfaceFactory.h"
#include "Models/CanMessage.h"

class CommunicationManager : public QObject
{
    Q_OBJECT

public:
    CommunicationManager(CanBusProvider provider, QObject *parent = nullptr);

    void connect(QString ipAddress, QString port, BaudRate baudRate);
    void disconnect();

    void sendDirectCanMessage(std::vector<uint8_t> data, uint32_t id);

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
    std::shared_ptr<CanBusInterface> canBusInterface;
};

#endif // COMMUNICATIONMANAGER_H
