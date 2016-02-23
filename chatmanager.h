#ifndef CHATMANAGER_H
#define CHATMANAGER_H

#include <QTimer>
#include <QList>
#include <QUdpSocket>
#include <QTcpServer>
#include <QTcpSocket>
#include <QQmlListProperty>
#include "userlistmodel.h"

#define PORT 8809
#define CHECK_DELAY 1000
#define DISCONNECT_DELAY 2000

class ChatManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    QString m_name;

    QUdpSocket m_udpSocket;
    QTcpServer m_server;
    QMap<quint32, User*> m_usersMap;
    QTimer m_timer;
    void addUser(QHostAddress addr, QString name);

public:
    explicit ChatManager(QObject *parent = 0);
    ~ChatManager();
    QString name() const;
    Q_INVOKABLE void sendMessage(QString msg, quint32 userIP);

    UserListModel userListModel;

signals:
    void receiveMessage(QString msg, quint32 userIP);
    void nameChanged(QString name);

public slots:
    void readDatagrams();
    void timerSlot();
    void newConnection();
    void readyRead();
    void acceptError(QAbstractSocket::SocketError socketError);
    void setName(QString name);
    void outSocketConnected();
    void outSocketError(QAbstractSocket::SocketError socketError);
    void inSocketError(QAbstractSocket::SocketError socketError);
    void inSocketDisconnect();
    void outSocketDisconnect();
};




#endif // CHATMANAGER_H
