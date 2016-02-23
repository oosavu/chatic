#ifndef USERLISTMODEL_H
#define USERLISTMODEL_H

#include <QAbstractListModel>
#include <QHostAddress>
#include <QTcpServer>
#include <QTime>

struct User{
    QString nickname;
    QTcpSocket* inSocket = 0;
    QTcpSocket* outSocket = 0;
    QTime delay;
    QHostAddress host;
    bool ready = false;
};

struct UserData{
    QString ip;
    QString nickname;
    quint32 host;
    UserData(QHostAddress _host, QString _nickname  = ""){
        ip = _host.toString();
        nickname = _nickname;
        host = _host.toIPv4Address();
    }
};

class UserListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit UserListModel(QObject *parent = 0);
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    enum UserRoles {
        IpRole = Qt::UserRole + 1,
        NameRole,
        HostRole
    };

    void sync(QMap<quint32, User*> users);

protected:
    QHash<int, QByteArray> roleNames() const;
private:
    QList<UserData> m_users;
};

#endif // USERLISTMODEL_H
