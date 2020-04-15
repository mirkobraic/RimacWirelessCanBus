#ifndef ISOTPCANPROVIDER_H
#define ISOTPCANPROVIDER_H

#include "isotp_api/can/can_layer_message.hpp"
#include "isotp_api/can/isotp_can_provider.hpp"
#include "CanLayer/CanBusInterfaceFactory.h"

class IsotpCanProvider : public isotp::can_provider
{
public:
    IsotpCanProvider(std::shared_ptr<CanBusInterface> canBusInterface);

    void send_can_message(std::unique_ptr<isotp::can_layer_message> message) override;
    void set_received_message_callback(std::function<void(std::unique_ptr<isotp::can_layer_message>)> callback) override;

private:
    std::shared_ptr<CanBusInterface> canBusInterface;
};

#endif // ISOTPCANPROVIDER_H
