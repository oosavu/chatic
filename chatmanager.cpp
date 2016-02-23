#include "chatmanager.h"


ChatManager::ChatManager(QObject *parent) : QObject(parent)
{
    m_udpSocket.bind(QHostAddress::Any, PORT);
    connect(&m_udpSocket, SIGNAL(readyRead()),this, SLOT(readDatagrams()));
    connect(&m_timer,SIGNAL(timeout()),this,SLOT(timerSlot()));
    connect(&m_server, SIGNAL(newConnection()), this, SLOT(newConnection()));
    connect(&m_server, SIGNAL(acceptError(QAbstractSocket::SocketError)), this,
            SLOT(acceptError(QAbstractSocket::SocketError)));
    m_timer.start(CHECK_DELAY);
    m_server.listen(QHostAddress::Any, PORT);
    m_name = "";
}

ChatManager::~ChatManager()
{

}

void ChatManager::sendMessage(QString msg, quint32 userIP)
{
    if(m_usersMap.contains(userIP)){
        User* user = m_usersMap[userIP];
        user->outSocket->write(msg.toUtf8());
        user->outSocket->flush();
    }
}

void ChatManager::outSocketConnected()
{
    qDebug() << "out socket connected";
    QObject * object = QObject::sender();
    if (!object)
        return;
    QTcpSocket * soc = static_cast<QTcpSocket *>(object);
    quint32 ip = soc->peerAddress().toIPv4Address();
    if(m_usersMap.contains(ip)){
        m_usersMap[ip]->outReady = true;
    }
}


void ChatManager::outSocketError(QAbstractSocket::SocketError socketError)
{
    qDebug() << "out socket error";
    QObject * object = QObject::sender();
    if (!object)
        return;
    QTcpSocket * soc = static_cast<QTcpSocket *>(object);
    quint32 ip = soc->peerAddress().toIPv4Address();
    if(m_usersMap.contains(ip)){
//        m_usersMap[ip]->outReady = false;
//        soc->deleteLater();
        m_usersMap[ip]->deleteSockets();
        delete m_usersMap[ip];
        m_usersMap.remove(ip);

    }
}

void ChatManager::inSocketError(QAbstractSocket::SocketError socketError)
{
    qDebug() << "in socket connected";
    QObject * object = QObject::sender();
    if (!object)
        return;
    QTcpSocket * soc = static_cast<QTcpSocket *>(object);
    quint32 ip = soc->peerAddress().toIPv4Address();
    if( m_usersMap.contains(ip)){
//        m_usersMap[ip]->inReady = false;
//        soc->deleteLater();
        m_usersMap[ip]->deleteSockets();
        delete m_usersMap[ip];
        m_usersMap.remove(ip);
    }
}

void ChatManager::inSocketDisconnect()
{
    qDebug() << " in cocket disconnected ";
    QObject * object = QObject::sender();
    if (!object)
        return;
    QTcpSocket * soc = static_cast<QTcpSocket *>(object);
    quint32 ip = soc->peerAddress().toIPv4Address();
    if( m_usersMap.contains(ip)){
//        m_usersMap[ip]->inReady = false;
//        soc->deleteLater();
        //m_usersMap[ip]->deleteSockets();
//        m_usersMap[ip]->
        delete m_usersMap[ip];
        m_usersMap.remove(ip);
    }
}

void ChatManager::outSocketDisconnect()
{
    qDebug() << " out cocket disconnected ";
    QObject * object = QObject::sender();
    if (!object)
        return;
    QTcpSocket * soc = static_cast<QTcpSocket *>(object);
    quint32 ip = soc->peerAddress().toIPv4Address();
    if(m_usersMap.contains(ip)){
        m_usersMap[ip]->deleteSockets();
        delete m_usersMap[ip];
        m_usersMap.remove(ip);
    }
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
                addUser(sender,message.remove(0,5));
            }
        }
        qDebug() << "datagrammmm!!! " << sender.toString() << " " << QString::fromUtf8(datagram);
    }
}


void ChatManager::addUser(QHostAddress addr, QString name)
{
    quint32 senderIP = addr.toIPv4Address();
    User* user = new User();
    user->nickname =name;
    user->host = addr;
    user->outSocket = new QTcpSocket();
    connect(user->outSocket, SIGNAL(connected()), this, SLOT(outSocketConnected()));
    connect(user->outSocket, SIGNAL(error(QAbstractSocket::SocketError)),this,
            SLOT(outSocketError(QAbstractSocket::SocketError)));
    user->outSocket->connectToHost(addr,PORT);
    m_usersMap[senderIP] = user;
}

void ChatManager::disconnectSockets()
{
    for (auto it = m_usersMap.begin(); it != m_usersMap.end();++it){
        if(it.value()->inSocket){
            disconnect(it.value()->inSocket,SLOT(inSocketDisconnect()));
        }
        if(it.value()->outSocket){
            disconnect(it.value()->outSocket,SLOT(outSocketDisconnect()));
        }
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
            it.value()->deleteSockets();
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
    qDebug() <<"new connection";
    QTcpSocket* soc = m_server.nextPendingConnection();
    quint32 ip = soc->peerAddress().toIPv4Address();
    if(!m_usersMap.contains(ip))
            addUser(soc->peerAddress(),"");
    m_usersMap[ip]->inSocket = soc;
    m_usersMap[ip]->inReady = true;
    connect(soc, SIGNAL(readyRead()),this, SLOT(readyRead()));
    connect(soc, SIGNAL(error(QAbstractSocket::SocketError)),this,
            SLOT(inSocketError(QAbstractSocket::SocketError)));
    connect(soc, SIGNAL(disconnected()),this, SLOT(inSocketDisconnect()));
}

void ChatManager::readyRead()
{
    QObject * object = QObject::sender();
    qDebug() <<"receive message";
    if (!object)
        return;
    QTcpSocket * soc = static_cast<QTcpSocket *>(object);
    quint32 ip = soc->peerAddress().toIPv4Address();
    if(m_usersMap.contains(ip)){
        emit receiveMessage(soc->readAll(),ip);
    }
}

void ChatManager::acceptError(QAbstractSocket::SocketError socketError)
{
    qDebug() <<"server accept error";
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


