#include "CanMessage.h"

uint32_t CanMessage::getId() const
{
    return id;
}

uint8_t CanMessage::getDlc() const
{
    return dlc;
}

std::vector<uint8_t> CanMessage::getData() const
{
    return data;
}

void CanMessage::setData(const std::vector<uint8_t> &value)
{
    data = value;
}

qint64 CanMessage::getTimestamp() const
{
    return timestamp;
}

void CanMessage::setTimestamp(const qint64 &value)
{
    timestamp = value;
}

void CanMessage::setId(const uint32_t &value)
{
    id = value;
}

void CanMessage::setDlc(const uint8_t &value)
{
    dlc = value;
}

CanMessage::CanMessage(uint32_t id, uint8_t dlc, std::vector<uint8_t> data)
{
    if (id > maxExtCanId || id == 0) {
        throw std::invalid_argument("Invalid ID");
    }
    if (dlc > 8) {
        throw std::invalid_argument("Data length too big");
    }

    this->id = id;
    this->dlc = dlc;
    this->data = data;
}

bool CanMessage::isExtended()
{
    return id > maxStdCanId;
}
