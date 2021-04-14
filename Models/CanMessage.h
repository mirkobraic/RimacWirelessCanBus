#ifndef CANMESSAGE_H
#define CANMESSAGE_H

#include <QtGlobal>
#include <stdexcept>
#include <vector>

class CanMessage
{
public:
    CanMessage(uint32_t id, uint8_t dlc, std::vector<uint8_t> data);
    CanMessage() { id = 0; dlc = 0; data = {}; };

    const static uint32_t maxStdCanId = 0x7FF;
    const static uint32_t maxExtCanId = 0x1FFFFFFF;

    bool isExtended();

    uint32_t getId() const;
    void setId(const uint32_t &value);

    uint8_t getDlc() const;
    void setDlc(const uint8_t &value);

    std::vector<uint8_t> getData() const;
    void setData(const std::vector<uint8_t> &value);

    qint64 getTimestamp() const;
    void setTimestamp(const qint64 &value);

private:
    uint32_t id;
    uint8_t dlc;
    std::vector<uint8_t> data;
    qint64 timestamp;
};

#endif // CANMESSAGE_H
