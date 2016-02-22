#include "chatmanager.h"

ChatManager::ChatManager(QObject *parent) : QObject(parent)
{
    m_udpSocket.bind(QHostAddress::Any, PORT);
    connect(&m_udpSocket, SIGNAL(readyRead()),this, SLOT(readDatagrams()));
    connect(&m_timer,SIGNAL(timeout()),this,SLOT(timerSlot()));
    connect(&m_server, SIGNAL(newConnection()), this, SLOT(newConnection()));
    m_timer.start(CHECK_DELAY);
    m_server.listen(QHostAddress::Any, PORT);
    m_name = "asdfasdf";
}

void ChatManager::sendMessage(QString msg, quint32 userIP)
{
//    if(m_usersMap.contains(userIP)){
//        User* user = m_usersMap[userIP];
//        qDebug()<< "go write!";
//        user->sendSocket->write(msg.toUtf8());
//        qDebug()<< "writed!";
//    }
}


void ChatManager::readDatagrams()
{
    while (m_udpSocket.hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(m_udpSocket.pendingDatagramSize());
        QHostAddress sender;
        m_udpSocket.readDatagram(datagram.data(), datagram.size(),&sender);
        QString message = QString::fromUtf8(datagram);
        quint32 senderIP = sender.toIPv4Address();
        if(message.startsWith("name:")){
            if(m_usersMap.contains(senderIP)){
                //qDebug() << "read " << m_usersMap[senderIP]->aliveDelay.elapsed();
                m_usersMap[senderIP]->delay.restart();
            }
            else{
                User* user = new User();
                message.truncate(5);
                user->nickname = message;
                user->host = sender;
                user->ready = true;
                //user->outSocket = new QTcpSocket();
                m_usersMap[senderIP] = user;

            }
        }
        qDebug() << "datagrammmm!!! " << sender.toString() << " " << QString::fromUtf8(datagram);
    }
}

void ChatManager::timerSlot()
{
    QString aliveString = "name:" + m_name;
    QByteArray aliveData = aliveString.toUtf8();
    m_udpSocket.writeDatagram(aliveData.data(), aliveData.size(), QHostAddress::Broadcast, PORT);

    for (auto it = m_usersMap.begin(); it != m_usersMap.end();){
        //qDebug() << "check " <<it.value()->aliveDelay.elapsed();
        if (it.value()->delay.elapsed() > DISCONNECT_DELAY){
            delete it.value();
            it = m_usersMap.erase(it);
        }
        else{
            ++it;
        }
    }
    userListModel.sync(m_usersMap);
}

void ChatManager::newConnection()
{
    QTcpSocket* soc = m_server.nextPendingConnection();
    quint32 ip = soc->peerAddress().toIPv4Address();
    if(m_usersMap.contains(ip)){
        m_usersMap[ip]->inSocket = soc;
        connect(soc, SIGNAL(readyRead()),this, SLOT(readyRead()));
    }
}

void ChatManager::readyRead()
{
    QObject * object = QObject::sender();
    if (!object)
        return;
    QTcpSocket * soc = static_cast<QTcpSocket *>(object);
    quint32 ip = soc->peerAddress().toIPv4Address();
    if(m_usersMap.contains(ip)){
        //qDebug() <<"receive"<< soc->readAll();
        emit receiveMessage(soc->readAll(),ip);
    }
}

void ChatManager::setName(QString name)
{
    if (m_name == name)
        return;

    m_name = name;
    emit nameChanged(name);
}

QString ChatManager::name() const
{
    return m_name;
}


