#ifndef CHATMANAGER_H
#define CHATMANAGER_H

#include <QTimer>
#include <QList>
#include <QUdpSocket>
#include <QTcpServer>
#include <QTcpSocket>
#include <QQmlListProperty>
#include <QNetworkInterface>
#include "userlistmodel.h"
#include "tinyaes.h"

#define PORT 8809
#define CHECK_DELAY 1000
#define DISCONNECT_DELAY 2000
#define AES_KEY "587b14e9ef30e95b64dc5ec71230197a"

union MsgHeader{
    char data[12];
    struct{
        quint32 head; //=0xFFFFFFFF
        quint32 length;
        quint32 tail; //=0xFFFFFFFF
    };
};

class ChatManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString selfIp READ selfIp NOTIFY selfIpChanged)
    QString m_name;

    QUdpSocket m_udpSocket;
    QTcpServer m_server;
    QMap<quint32, User*> m_usersMap;
    QTimer m_timer;
    void addUser(QHostAddress addr, QString name);
    bool getSelfHost();
    QHostAddress m_selfHost;
    QString m_selfIp;
    TinyAES m_crypto;
    QByteArray m_aesKey;

public:
    explicit ChatManager(QObject *parent = 0);
    ~ChatManager();
    QString name() const;
    Q_INVOKABLE void sendMessage(QString msg, quint32 userIP);

    UserListModel userListModel;

    QString selfIp() const;

signals:
    void receiveMessage(QString msg, quint32 userIP);
    void nameChanged(QString name);
    void selfIpChanged(QString selfIp);

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
