#ifndef WIICOMCSVPARSER_H
#define WIICOMCSVPARSER_H

#include "isotp_api/can/can_layer_message.hpp"
#include <QVector>

class WiicomCSVParser
{
public:
    WiicomCSVParser();

    QVector<isotp::can_layer_message> parseInput(QByteArray input);
    QByteArray convertMessage(isotp::can_layer_message msg, bool isExtended);
};

#endif // WIICOMCSVPARSER_H
