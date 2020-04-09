#ifndef CANMESSAGESLISTMODEL_H
#define CANMESSAGESLISTMODEL_H

#include <QAbstractListModel>
#include "CanMessage.h"

class CanMessageListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit CanMessageListModel(QObject *parent = nullptr);

    enum Roles {
        CanId = Qt::UserRole,
        CanData
    };

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    void addMessage(const CanMessage &message);

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Remove data:
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    // QML variable names
    virtual QHash<int, QByteArray> roleNames() const override;

    void removeAll();

private:
    QString formatCanId(CanMessage message) const;
    QString formatCanData(CanMessage messsage) const;

    QList<CanMessage> messages;
};

#endif // CANMESSAGESLISTMODEL_H
