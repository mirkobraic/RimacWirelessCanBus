#include "CanMessage.h"

uint CanMessage::getId() const
{
    return id;
}

quint8 CanMessage::getLength() const
{
    return length;
}

QByteArray CanMessage::getData() const
{
    return data;
}

CanMessage::CanMessage(uint id, quint8 length, QByteArray data)
{
    if (id > maxExtendedCanId) {
        throw IdOutOfRange;
    }
    if (length > 8) {
        throw DataOutOfRange;
    }

    this->id = id;
    this->length = length;
    this->data = data;
}
