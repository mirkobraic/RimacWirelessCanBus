#include "CanMessageListModel.h"

CanMessageListModel::CanMessageListModel(QObject *parent)
    : QAbstractListModel(parent)
{
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
    default:
        return QVariant();
    }
}

void CanMessageListModel::addMessage(const CanMessage &message)
{
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

QHash<int, QByteArray> CanMessageListModel::roleNames() const
{
    QHash<int, QByteArray> names;
    names[CanId] = "canId";
    names[CanData] = "canData";
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
    while (hexId.length() < desiredLength){
        hexId.prepend("0");
    }
    return hexId.toUpper();
}

QString CanMessageListModel::formatCanData(CanMessage message) const
{
    QString data = message.getData().toHex();
    // adds space after every two characters
    data.replace(QRegularExpression("(.{2})"), "\\1 ");
    return data.toUpper();
}
