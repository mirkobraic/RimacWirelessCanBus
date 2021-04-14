#include "WiicomCSVParser.h"

WiicomCSVParser::WiicomCSVParser()
{ }

QVector<CanMessage> WiicomCSVParser::parseInput(QByteArray input)
{
    QVector<CanMessage> output = {};

    QList<QByteArray> messages = input.split('#');
    messages.removeLast();

    for(QByteArray msg : messages) {
        QList<QByteArray> msgParts = msg.split(',');
        msgParts.removeLast();

        if (msgParts.count() < 4 || msgParts[0] != "*") {
            continue;
        }

        uint32_t id = msgParts[1].toUInt(nullptr, 16);
        uint8_t dlc = msgParts[3].toUInt(nullptr, 16);
        std::vector<uint8_t> data = {};
        for(int i = 4; i < msgParts.count(); i++) {
            data.push_back(msgParts[i].toUInt(nullptr, 16));
        }

        CanMessage outputMsg = CanMessage(id, dlc, data);
        if (outputMsg.getData().size() == dlc) {
            output.append(outputMsg);
        }
    }

    return output;
}

QByteArray WiicomCSVParser::convertMessage(CanMessage msg, bool isExtended)
{
    QByteArray output = QByteArray();
    output.append("*,");

    QString id = QString::number(msg.getId(), 16);
    output.append(id + ",");

    QString flag = isExtended ? "01" : "00";
    output.append(flag + ",");

    QString dlc = QString::number(msg.getData().size(), 16);
    output.append(dlc + ",");

    for (size_t i = 0; i < msg.getData().size(); i++) {
        QString byte = QString::number(msg.getData()[i], 16);
        output.append(byte + ",");
    }

    output.append("#");
    qDebug() << "Sending.. " << output;
    return output;
}
