#ifndef WIICOMWIRELESSCAN_H
#define WIICOMWIRELESSCAN_H

#include <QTcpSocket>
#include "../CanBusInterface.h"
#include "WiicomCSVParser.h"

class WiicomWirelessCan : public CanBusInterface
{
    Q_OBJECT
public:
    WiicomWirelessCan();
    ~WiicomWirelessCan();

    void connectToDevice(QString deviceIpAddress, QString port, BaudRate baudRate) override;
    void disconnectFromDevice() override;
    void sendCanMessage(isotp::can_layer_message &message) override;

public slots:
    void connected();
    void disconnected();
    void readyRead();
    void errorOccurred(QAbstractSocket::SocketError);

private:
    QTcpSocket *socket = nullptr;

    WiicomCSVParser csvParser = WiicomCSVParser();
};

#endif // WIICOMWIRELESSCAN_H
