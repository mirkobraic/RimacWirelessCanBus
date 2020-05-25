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
    Q_PROPERTY(bool isRawCanEnabled READ getIsRawCanEnabled WRITE setIsRawCanEnabled NOTIFY isRawCanEnabledChanged)
    Q_PROPERTY(bool fetchingInProgress READ getFetchingInProgress WRITE setFetchingInProgress NOTIFY fetchingInProgressChanged)

    Q_INVOKABLE void connectTapped(int provider, QString ipAddress, QString port, int baudRate, const QVariantList& rxTxPairs);
    Q_INVOKABLE void disconnectTapped();

    Q_INVOKABLE void sendDirectCanMessage(QString messageId, const QVector<QString> &bytes);

    Q_INVOKABLE void checkVersion(int tx);

    bool getIsConnected() const;

    bool getIsRawCanEnabled() const;
    void setIsRawCanEnabled(bool value);

    bool getFetchingInProgress() const;
    void setFetchingInProgress(bool value);

signals:
    void connectionChanged();
    void isRawCanEnabledChanged();
    void showAlert(QString title, QString message);
    void fetchingInProgressChanged();

public slots:
      void onNewCanMessageRecieved(CanMessage message);
      void onShowAlert(QString title, QString message);
      void onToggleBusyIndicator(bool value);
      void onToggleConnection(bool value);

private:
    CanMessageListModel *recievedMessages = nullptr;

    CommunicationManager *communicationManager = nullptr;

    bool isConnected = false;
    bool fetchingInProgress = false;
    bool isRawCanEnabled = false;
};

#endif // VIEWCONTROLLER_H
