#include "IsotpCanProvider.h"

IsotpCanProvider::IsotpCanProvider(std::shared_ptr<CanBusInterface> canBusInterface)
{
    this->canBusInterface = canBusInterface;
}

void IsotpCanProvider::send_can_message(std::unique_ptr<isotp::can_layer_message> message)
{
    canBusInterface->sendCanMessage(*(message.get()));
}

void IsotpCanProvider::set_received_message_callback(std::function<void (std::unique_ptr<isotp::can_layer_message>)> callback)
{
    canBusInterface->messageRecievedUdsCallback = callback;
}
