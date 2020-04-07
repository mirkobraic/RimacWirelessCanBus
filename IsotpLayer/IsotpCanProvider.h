#ifndef ISOTPCANPROVIDER_H
#define ISOTPCANPROVIDER_H

#include "isotp_api/isotp_api.hpp"
#include "CanLayer/CanBusInterfaceFactory.h"

class IsotpCanProvider : public isotp::can_provider
{
public:
    IsotpCanProvider(CanBusProvider provider);

    void send_can_message(std::unique_ptr<isotp::can_layer_message> message) override;

    void set_received_message_callback(std::function<void(std::unique_ptr<isotp::can_layer_message>)>) override;

    void connect_to_can(QString channelName, BaudRate baudRate);

    void disconnect_from_can();

    void setCanProvider(CanBusProvider provider);

private:
    CanBusInterface *canBusInterface = nullptr;
};

#endif // ISOTPCANPROVIDER_H
