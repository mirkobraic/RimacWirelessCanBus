#ifndef CANBUSINTERFACE_H
#define CANBUSINTERFACE_H

#include <memory>
#include <functional>
#include <QObject>
#include "isotp_api/can/can_layer_message.hpp"

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
    virtual void sendCanMessage(isotp::can_layer_message &message) = 0;

    std::function<void(std::unique_ptr<isotp::can_layer_message>)> messageRecievedUdsCallback;
    std::function<void(uint32_t, std::vector<uint8_t>)> messageRecievedDirectCallback;

signals:
    void showAlert(QString title, QString message);

protected:
    uint32_t maxStdCanId = 0x7FF;
};

#endif // CANBUSINTERFACE_H
