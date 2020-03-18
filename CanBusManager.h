#ifndef CANBUSMANAGER_H
#define CANBUSMANAGER_H

#include <QObject>
#include "CanBusInterfaceFactory.h"
#include "CanMessage.h"

class CanBusManager : public QObject {
    Q_OBJECT
public:
    explicit CanBusManager(QObject *parent = nullptr);
    CanBusManager(CanBusProvider, QObject *parent = nullptr);

    enum ConnectionStatus {
        NotConnected,
        Connecting,
        Connected
    };

    Q_ENUM(ConnectionStatus)
    Q_PROPERTY(ConnectionStatus connectionStatus READ getConnectionStatus NOTIFY connectionStatusChanged)

    Q_INVOKABLE void connectTapped();
    Q_INVOKABLE void disconnectTapped();
    Q_INVOKABLE void sendTapped(QString messageId, QString messageData);

    ConnectionStatus getConnectionStatus() const;

signals:
    void connectionStatusChanged();
    void addMessage(QString id, QString data);
    void invalidCanId();
    void invalidCanData();

public slots:
      void dataFrameRecieved(CanMessage message);

private:
    CanBusInterface *canBusInterface = nullptr;

    ConnectionStatus connectionStatus = NotConnected;
};

#endif // CANBUSMANAGER_H
