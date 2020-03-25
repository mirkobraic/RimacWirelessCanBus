#ifndef CANBUSMANAGER_H
#define CANBUSMANAGER_H

#include <QObject>
#include <QDebug>
#include <QRegularExpression>
#include "CanBusInterfaceFactory.h"
#include "CanMessageListModel.h"

class CanBusManager : public QObject {
    Q_OBJECT
public:
    explicit CanBusManager(QObject *parent = nullptr);
    CanBusManager(CanBusProvider provider, CanMessageListModel *recievedMessages, QObject *parent = nullptr);
    ~CanBusManager();

    Q_PROPERTY(bool isConnected READ getIsConnected NOTIFY connectionChanged)

    Q_INVOKABLE void connectTapped();
    Q_INVOKABLE void disconnectTapped();
    Q_INVOKABLE void sendTapped(QString messageId, QString messageData);

    bool getIsConnected() const;

signals:
    void connectionChanged();

public slots:
      void dataFrameRecieved(CanMessage message);

private:
    CanBusInterface *canBusInterface = nullptr;
    CanMessageListModel *recievedMessages = nullptr;

    bool isConnected = false;
    QRegularExpression hexMatcher = QRegularExpression("^[0-9A-F]{2,16}$", QRegularExpression::CaseInsensitiveOption);
};

#endif // CANBUSMANAGER_H
