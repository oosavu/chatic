#ifndef USER_H
#define USER_H

#include <QObject>
#include <QTime>
#include <QTcpSocket>
#include <QHostAddress>

class User : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ getName NOTIFY nameChanged)
    Q_PROPERTY(quint32 ip READ getIp NOTIFY ipChanged)
    QString m_name;
    quint32 m_ip;

public:
    explicit User(QHostAddress ip = QHostAddress() ,QString name = "",QObject *parent = 0);
    //explicit User(QObject *parent = 0){ qDebug() << "ERRRROR";}

    QString getName() const
    {
        return m_name;
    }

    QTcpSocket* sendSocket = 0;
    QTcpSocket* receiveSocket = 0;

    QTime aliveDelay;

    quint32 getIp() const
    {
        return m_ip;
    }

signals:

    void nameChanged(QString name);

    void ipChanged(quint32 ip);

public slots:

};

#endif // USER_H
