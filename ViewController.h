#ifndef VIEWCONTROLLER_H
#define VIEWCONTROLLER_H

#include <QObject>
#include <QDebug>
#include "Models/CanMessageListModel.h"
#include "IsotpLayer/IsotpManager.h"

class ViewController : public QObject
{
    Q_OBJECT

public:
    explicit ViewController(QObject *parent = nullptr);
    ViewController(CanMessageListModel *recievedMessages, QObject *parent = nullptr);
    ~ViewController();

    Q_PROPERTY(bool isConnected READ getIsConnected NOTIFY connectionChanged)

    Q_INVOKABLE void connectTapped();
    Q_INVOKABLE void disconnectTapped();
    Q_INVOKABLE void sendTapped(QString messageId, const QVector<QString> &bytes);

    bool getIsConnected() const;

signals:
    void connectionChanged();

public slots:
      void recievedMessageHandler(CanMessage message);

private:
    CanMessageListModel *recievedMessages = nullptr;

    IsotpManager *isotpManager = nullptr;

    bool isConnected = false;
};

#endif // VIEWCONTROLLER_H
