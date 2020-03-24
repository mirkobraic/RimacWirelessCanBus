#ifndef CANMESSAGE_H
#define CANMESSAGE_H

#include <QtGlobal>
#include <QByteArray>

class CanMessage {
    const uint32_t maxCanId = 0x7FF;
    const uint32_t maxExtendedCanId = 0x1fffffff;

    uint32_t id;
    uint8_t dlc;
    QByteArray data;

public:
    CanMessage(uint32_t id, uint8_t dlc, QByteArray data);

    uint32_t getId() const;
    uint8_t getDlc() const;
    QByteArray getData() const;
};

enum CanMessageException {
    IdOutOfRange,
    DataOutOfRange
};

#endif // CANMESSAGE_H
