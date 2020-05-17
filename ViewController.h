#ifndef VIEWCONTROLLER_H
#define VIEWCONTROLLER_H

#include <QObject>
#include <QDebug>
#include "Models/CanMessageListModel.h"
#include "CommunicationManager.h"

class ViewController : public QObject
{
    Q_OBJECT

public:
    explicit ViewController(QObject *parent = nullptr);
    ViewController(CanMessageListModel *recievedMessages, QObject *parent = nullptr);
    ~ViewController();

    Q_PROPERTY(bool isConnected READ getIsConnected NOTIFY connectionChanged)

    Q_INVOKABLE void connectTapped(int provider, const QVariantList& rxTxPairs);
    Q_INVOKABLE void disconnectTapped();

    Q_INVOKABLE void sendDirectCanMessage(QString messageId, const QVector<QString> &bytes);

    Q_INVOKABLE void checkVersion(int tx);

    bool getIsConnected() const;

signals:
    void connectionChanged();
    void showAlert(QString title, QString message);

public slots:
      void onNewCanMessageRecieved(CanMessage message);

private:
    CanMessageListModel *recievedMessages = nullptr;

    CommunicationManager *communicationManager = nullptr;

    bool isConnected = false;
};

#endif // VIEWCONTROLLER_H
