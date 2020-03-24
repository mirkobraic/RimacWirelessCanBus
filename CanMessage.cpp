#include "CanMessage.h"

uint CanMessage::getId() const
{
    return id;
}

quint8 CanMessage::getDlc() const
{
    return dlc;
}

QByteArray CanMessage::getData() const
{
    return data;
}

CanMessage::CanMessage(uint id, quint8 dlc, QByteArray data)
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
