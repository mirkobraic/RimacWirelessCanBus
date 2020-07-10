#ifndef CANBUSINTERFACE_H
#define CANBUSINTERFACE_H

#include <memory>
#include <functional>
#include <QObject>
#include "Models/CanMessage.h"

enum BaudRate {
    Baud_125 = 0,
    Baud_250 = 1,
    Baud_500 = 2,
    Baud_1000 = 3
};

class CanBusInterface : public QObject
{
    Q_OBJECT
public:
    virtual ~CanBusInterface() {}

    virtual void connectToDevice(QString deviceIpAddress, QString port, BaudRate baudRate) = 0;
    virtual void disconnectFromDevice() = 0;
    virtual void sendCanMessage(CanMessage &message) = 0;

signals:
    void newDirectCanMessage(uint32_t, std::vector<uint8_t>);
    void showAlert(QString title, QString message);
    void fetchingInProgress(bool value);
    void toggleConnection(bool value);

protected:
    uint32_t maxStdCanId = 0x7FF;
};

#endif // CANBUSINTERFACE_H
