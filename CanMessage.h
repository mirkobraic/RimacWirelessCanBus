#ifndef CANMESSAGE_H
#define CANMESSAGE_H

#include <QtGlobal>
#include <QByteArray>

class CanMessage {
    const uint maxCanId = 0x7FF;
    const uint maxExtendedCanId = 0x1fffffff;

    uint id;
    quint8 dlc;
    QByteArray data;

public:
    CanMessage(uint id, quint8 dlc, QByteArray data);

    uint getId() const;
    quint8 getDlc() const;
    QByteArray getData() const;
};

enum CanMessageException {
    IdOutOfRange,
    DataOutOfRange
};

#endif // CANMESSAGE_H
