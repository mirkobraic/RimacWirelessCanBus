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
    if (id > maxExtCanId || id == 0) {
        throw std::invalid_argument("Invalid ID");
    }
    if (dlc > 8) {
        throw std::invalid_argument("Data length too big");
    }

    this->id = id;
    this->dlc = dlc;
    this->data = QByteArray(data.data(), dlc);  // performs deep copy
}

bool CanMessage::isExtended()
{
    return id > maxStdCanId;
}
