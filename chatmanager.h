#ifndef CHATMANAGER_H
#define CHATMANAGER_H

#include <QTimer>
#include <QList>
#include <QUdpSocket>
#include <QTcpServer>
#include <QQmlListProperty>
#include "user.h"


#define PORT 8809
#define CHECK_DELAY 1000
#define DISCONNECT_DELAY 3000

class ChatManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<User> users READ getUsers NOTIFY usersChanged)
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)

    QUdpSocket m_udpSocket;
    //QQmlListProperty<User> m_users;
    QString m_name;
    QTcpServer m_server;
    QList<User*> m_usersList;
    QMap<quint32, User*> m_usersMap;
    QTimer m_timer;

public:
    explicit ChatManager(QObject *parent = 0);

    Q_INVOKABLE void sendMessage(QString msg, quint32 userIP);

    QQmlListProperty<User> getUsers()
    {
        qDebug() << "getusers";
        //return m_users;
        //return QQmlListProperty<User>(this,m_usersList);
        return QQmlListProperty<User>(this,0,&usersCount,&userAt);
    }

    QString name() const
    {
        return m_name;
    }

    static int usersCount(QQmlListProperty<User>*list)
    {
        qDebug() << "getcount";
        ChatManager *chatManager = qobject_cast<ChatManager*>(list->object);
        if (chatManager)
            return chatManager->m_usersList.count();
        return 0;
    }

    static User* userAt(QQmlListProperty<User> *list, int i)
    {

        ChatManager *chatManager = qobject_cast<ChatManager*>(list->object);
        if (chatManager){
            qDebug() << "getat. size:"<<chatManager->m_usersList.size();
            if(chatManager->m_usersList.size()> i)
                return chatManager->m_usersList.at(i);
        }
        return 0;

    }

signals:
    void getUdp(QString message);
    void usersChanged();
    void receiveMessage(QString msg, quint32 userIP);
    void nameChanged(QString name);

public slots:
    void readDatagrams();
    void timerSlot();
    void newConnection();
    void readyRead();
};

#endif // CHATMANAGER_H
