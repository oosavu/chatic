#include "user.h"

User::User(QHostAddress ip, QString name, QObject *parent) : QObject(parent)
{
    m_ip = ip.toIPv4Address();
    name == ""? m_name = name: m_name = "m_ip";
    aliveDelay.restart();
    sendSocket = new QTcpSocket();
    receiveSocket = new QTcpSocket();
}
