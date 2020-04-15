#ifndef CANBUSINTERFACE_H
#define CANBUSINTERFACE_H

#include <memory>
#include <functional>
#include "isotp_api/can/can_layer_message.hpp"

enum BaudRate {
    Baud_125, Baud_250, Baud_500, Baud_1000
};

class CanBusInterface
{
public:
    virtual ~CanBusInterface() {}

    virtual void connect(std::string channelName, BaudRate baudRate) = 0;
    virtual void disconnect() = 0;
    virtual void sendCanMessage(isotp::can_layer_message &message) = 0;

    std::function<void(std::unique_ptr<isotp::can_layer_message>)> recievedMessageCallback;

protected:
    uint32_t maxStdCanId = 0x7FF;
};

#endif // CANBUSINTERFACE_H
