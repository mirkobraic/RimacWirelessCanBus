#include "WiicomCSVParser.h"
#include <QDebug>

WiicomCSVParser::WiicomCSVParser()
{ }

QVector<isotp::can_layer_message> WiicomCSVParser::parseInput(QByteArray input)
{
    QVector<isotp::can_layer_message> output = QVector<isotp::can_layer_message>();

    QList<QByteArray> messages = input.split('#');
    messages.removeLast();

    for(QByteArray msg : messages) {
        QList<QByteArray> msgParts = msg.split(',');
        msgParts.removeLast();

        if (msgParts.count() < 4 || msgParts[0] != "*") {
            continue;
        }

        isotp::can_layer_message outputMsg = isotp::can_layer_message();
        outputMsg.id = msgParts[1].toUInt(nullptr, 16);
//        int type = msgParts[2].toInt(nullptr, 16);
        uint dlc = msgParts[3].toUInt(nullptr, 16);

        for(int i = 4; i < msgParts.count(); i++) {
            outputMsg.data.push_back(msgParts[i].toUInt(nullptr, 16));
        }
        if (outputMsg.data.size() == dlc) {
            output.append(outputMsg);
        }
    }

    return output;
}

QByteArray WiicomCSVParser::convertMessage(isotp::can_layer_message msg, bool isExtended)
{
    QByteArray output = QByteArray();
    output.append("*,");

    QString id = QString::number(msg.id, 16);
    output.append(id + ",");

    QString flag = isExtended ? "00" : "01";
    output.append(flag + ",");

    QString dlc = QString::number(msg.data.size(), 16);
    output.append(dlc + ",");

    for (size_t i = 0; i < msg.data.size(); i++) {
        QString byte = QString::number(msg.data[i], 16);
        output.append(byte + ",");
    }

    output.append("#");
    qDebug() << "Output.. " << output;
    return output;
}
