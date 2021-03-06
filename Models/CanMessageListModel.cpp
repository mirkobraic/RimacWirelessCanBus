#include "CanMessageListModel.h"

CanMessageListModel::CanMessageListModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

void CanMessageListModel::clearMessages()
{
    removeRows(0, rowCount());
}

void CanMessageListModel::toggleOverride(bool on)
{
    if (isOverrideOn == false && on == true) {
        // delete extra messages
        // start from the back and remove duplicates
        QVector<uint32_t> uniqueIds;
        for (int i = rowCount() - 1; i >= 0; i--) {
            uint32_t msgId = messages[i].getId();
            if (uniqueIds.contains(msgId)) {
                removeRow(i);
            } else {
                uniqueIds.append(msgId);
            }
        }
    }
    isOverrideOn = on;
}

int CanMessageListModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    return messages.count();
}

QVariant CanMessageListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    const CanMessage &message = messages[index.row()];
    switch(role) {
    case CanId:
        return QVariant(formatCanId(message));
    case CanData:
        return QVariant(formatCanData(message));
    case Timestamp:
        return QVariant(message.getTimestamp());
    default:
        return QVariant();
    }
}

void CanMessageListModel::addMessage(const CanMessage &message)
{
    if (isOverrideOn) {
        for (int i = 0; i < messages.count(); i++) {
            if (messages[i].getId() == message.getId()) {
                std::vector<uint8_t> data = message.getData();
                QVector<uint8_t> dataVec = QVector<uint8_t>::fromStdVector(data);
                QList<uint8_t> dataList = QList<uint8_t>::fromVector(dataVec);

                QList<QVariant> variantList;
                foreach(uint8_t byte, dataList) {
                    variantList << QVariant(byte);
                }

                QVariant variantData(variantList);
                setData(index(i, 0), variantData, CanData);

                QVariant variantTimestamp(message.getTimestamp());
                setData(index(i, 0), variantTimestamp, Timestamp);
                return;
            }
        }
    }

    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    messages.append(message);
    endInsertRows();
}

bool CanMessageListModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    // removes from the back so the index doesn't change
    for (int i = row + count - 1; i >= row; i--) {
        messages.removeAt(i);
    }
    endRemoveRows();
    return true;
}

bool CanMessageListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.row() >= messages.count()) {
        return false;
    }

    CanMessage &message = messages[index.row()];

    switch (role) {
    case CanId:
        message.setId(value.toUInt());
        break;
    case CanData: {
        QList variantList = value.toList();
        QList<uint8_t> dataList;
        foreach(QVariant v, variantList) {
            dataList << v.value<uint8_t>();
        }
        std::vector<uint8_t> data = std::vector<uint8_t>(dataList.begin(), dataList.end());
        message.setData(data);
        break;
    }
    case Timestamp:
        message.setTimestamp(value.toLongLong());
        break;

    }
    emit dataChanged(index, index, QVector<int>() << role);
    return true;
}

QHash<int, QByteArray> CanMessageListModel::roleNames() const
{
    QHash<int, QByteArray> names;
    names[CanId] = "CanId";
    names[CanData] = "CanData";
    names[Timestamp] = "Timestamp";
    return names;
}

void CanMessageListModel::removeAll()
{
    removeRows(0, messages.count());
}

QString CanMessageListModel::formatCanId(CanMessage message) const
{
    QString hexId;
    hexId.setNum(message.getId(), 16);
    int desiredLength = message.isExtended() ? 8 : 3;
    while (hexId.length() < desiredLength) {
        hexId.prepend("0");
    }
    QString output = hexId.toUpper();
    output.prepend("0x");
    return output;
}

QString CanMessageListModel::formatCanData(CanMessage message) const
{
    QString formatted;
    for(const uint8_t &byte: message.getData()) {
        if (byte < 16) {
            formatted += "0";
        }
        formatted += QString::number(byte, 16) + " ";
    }
    return formatted.toUpper();
}
