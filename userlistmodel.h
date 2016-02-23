#ifndef USERLISTMODEL_H
#define USERLISTMODEL_H

#include <QAbstractListModel>
#include <QHostAddress>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTime>

struct User{
    QString nickname;
    QTcpSocket* inSocket = 0;
    QTcpSocket* outSocket = 0;
    QTime delay;
    QHostAddress host;
    bool inReady = false;
    bool outReady = false;
    void deleteSockets(){
        if(inSocket != 0){
            //inSocket->disconnect();
            //QObject::disconnect(inSocket,);
            inSocket->deleteLater();
        }
        if(outSocket != 0){
            //outSocket->disconnect();
            //QObject::disconnect(outSocket);
            outSocket->deleteLater();
        }
    }

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
    Q_INVOKABLE QVariant data(int index, QString role ) const;
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
