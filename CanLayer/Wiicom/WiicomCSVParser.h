#ifndef WIICOMCSVPARSER_H
#define WIICOMCSVPARSER_H

#include "../../Models/CanMessage.h"
#include <QVector>
#include <QList>
#include <QDebug>

class WiicomCSVParser
{
public:
    WiicomCSVParser();

    QVector<CanMessage> parseInput(QByteArray input);
    QByteArray convertMessage(CanMessage msg, bool isExtended);
};

#endif // WIICOMCSVPARSER_H
