#include "IsotpCanProvider.h"

IsotpCanProvider::IsotpCanProvider(CanBusProvider provider)
{
    canBusInterface = CanBusInterfaceFactory::getInterfaceForProvider(provider);
}

void IsotpCanProvider::send_can_message(std::unique_ptr<isotp::can_layer_message> message)
{
    QByteArray data = QByteArray(reinterpret_cast<const char*>(message->data.data(), message->data.size()));
    uint8_t dlc = data.length();
    try {
        CanMessage msg(message->id, dlc, data);
        canBusInterface->sendCanMessage(msg);

    } catch (const std::exception& ex) {
        qDebug() << "Exception: " << ex.what();
    }
}

void IsotpCanProvider::set_received_message_callback(std::function<void (std::unique_ptr<isotp::can_layer_message>)> callback)
{
    // QUESTION: where to set callback and when to call it
}

void IsotpCanProvider::connect_to_can(QString channelName, BaudRate baudRate)
{
    canBusInterface->connect(channelName, baudRate);
}

void IsotpCanProvider::disconnect_from_can()
{
    canBusInterface->disconnect();
}

void IsotpCanProvider::setCanProvider(CanBusProvider provider)
{
    delete canBusInterface;
    canBusInterface = CanBusInterfaceFactory::getInterfaceForProvider(provider);
}
