#include "CanMessage.h"

uint32_t CanMessage::getId() const
{
    return id;
}

uint8_t CanMessage::getDlc() const
{
    return dlc;
}

QByteArray CanMessage::getData() const
{
    return data;
}

CanMessage::CanMessage(uint32_t id, uint8_t dlc, QByteArray data)
{
    if (id > maxExtendedCanId) {
        throw IdOutOfRange;
    }
    if (dlc > 8) {
        throw DataOutOfRange;
    }

    this->id = id;
    this->dlc = dlc;
    this->data = data;
}
