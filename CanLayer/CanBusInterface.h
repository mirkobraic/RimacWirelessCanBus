#ifndef CANBUSINTERFACE_H
#define CANBUSINTERFACE_H

#include <QObject>
#include <memory>
#include "../Models/CanMessage.h"
#include "isotp_api/can/can_layer_message.hpp"

enum BaudRate {
    Baud_125, Baud_250, Baud_500, Baud_1000
};

class CanBusInterface : public QObject
{
public:
    virtual ~CanBusInterface() {}

    virtual void connect(QString channelName, BaudRate baudRate) = 0;
    virtual void disconnect() = 0;
    virtual void sendCanMessage(CanMessage message) = 0;

    std::function<void(std::unique_ptr<isotp::can_layer_message>)> recievedMessageCallback;

signals:
    virtual void newDataFrame(CanMessage message) = 0;
};

#endif // CANBUSINTERFACE_H
