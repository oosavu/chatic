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
//интервал отправки сообщения по UDP
#define CHECK_DELAY 1000
//максимально допустимый интервал отсутствия сообщения по UDP
#define DISCONNECT_DELAY 5000
//ключ шифрования
#define AES_KEY "587b14e9ef30e95b64dc5ec71230197a"


/**
 * @brief The MsgHeader union
 * заголовок текстового сообщения, передаваемого по TCP. Содержит поле длинны сообщения,
 * а так же разделительные байты
 */
union MsgHeader{
    char data[12];
    struct{
        quint32 head; //=0xFFFFFFFF
        quint32 length;
        quint32 tail; //=0xFFFFFFFF
    };
};


/**
 * @brief The ChatManager class
 * Класс, предоставляющий основной интерфейс для QML-сцены,
 * а так же реализующий сетевое взаимодействие
 */
class ChatManager : public QObject
{
    Q_OBJECT
    // имя пользователя. может быть пустым.
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    // текстовое представление собственного IP-адреса
    Q_PROPERTY(QString selfIp READ selfIp NOTIFY selfIpChanged)

    QString m_name;
    QUdpSocket m_udpSocket; //сокет для приёма сигнализирующих сообщений от других пользователей
    QTcpServer m_server;    //сервер для приёма входящих соединений
    QMap<quint32, User*> m_usersMap; //таблица ip-пользователь. Динамический изменяется при подключении
                                     //новых пользователей
    QTimer m_timer;
    void addUser(QHostAddress addr, QString name);//инициализирует нового пользователя,
                                                  //отправляет ему запрос на соединение
    bool getSelfHost();
    QHostAddress m_selfHost;
    QString m_selfIp;
    TinyAES m_crypto;
    QByteArray m_aesKey;

public:
    explicit ChatManager(QObject *parent = 0);

    //интерфейс для посылки сообщения
    Q_INVOKABLE void sendMessage(QString msg, quint32 userIP);
    //динамический изменяемая модель с информацией о подключенных пользователях.
    UserListModel userListModel;

    QString name() const;
    QString selfIp() const;

signals:
    //Высылается при получении нового сообщения
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
