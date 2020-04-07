#ifndef CANMESSAGE_H
#define CANMESSAGE_H

#include <QtGlobal>
#include <QByteArray>

class CanMessage
{
public:
    CanMessage(uint32_t id, uint8_t dlc, QByteArray data);

    const static uint32_t maxStdCanId = 0x7FF;
    const static uint32_t maxExtCanId = 0x1FFFFFFF;

    bool isExtended();

    uint32_t getId() const;
    uint8_t getDlc() const;
    QByteArray getData() const;

private:
    uint32_t id;
    uint8_t dlc;
    QByteArray data;


};

#endif // CANMESSAGE_H
