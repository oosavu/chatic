#ifndef USERLISTMODEL_H
#define USERLISTMODEL_H

#include <QAbstractListModel>
#include <QHostAddress>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTime>



/**
 * @brief The User struct
 * Рабочая структура для пользователя.
 */
struct User{
    //Имя пользователя
    QString nickname;
    //Сокет, на который приходят сообщения от него
    QTcpSocket* inSocket = 0;
    //Сокет, на который посылаются сообщения для него
    QTcpSocket* outSocket = 0;
    //Время, прошедшее с последнего UDP-сообщения от него
    QTime delay;
    //адрес
    QHostAddress host;
    //Входящее соединение установлено
    bool inReady = false;
    //Исходящее соединение установлено
    bool outReady = false;
    //Буфер входящего потока байт от него
    //(на случай если TCP будет дробить сообщение на несколько пакетов)
    QByteArray buff;
    //Длинна ожидаемого от него сообщения
    int msgLength = 0;
    //Перед уничтожением структуры необходимо вызвать данный метод во избежании утечки памяти
    //и закрытия соединений
    void deleteSockets(){
        if(inSocket != 0)
            inSocket->deleteLater();
        if(outSocket != 0)
            outSocket->deleteLater();
    }

};

//структура для модели данных для QML
struct UserData{
    QString ip;
    QString nickname;
    quint32 host;
    UserData(QHostAddress _host, QString _nickname  = ""){
        ip = _host.toString();
        //ip.remove(0,7);
        nickname = _nickname;
        host = _host.toIPv4Address();
    }
};

/**
 * @brief The UserListModel class
 * Модель данных для QML.
 */
class UserListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit UserListModel(QObject *parent = 0);
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    //QML для моделей, проброшеных из C++ не позволяет вызвать метод get().
    //В связи с этим пришлось написать аналогичный метод
    Q_INVOKABLE QVariant data(int index, QString role ) const;
    //Получить индекс модели по IP
    Q_INVOKABLE QVariant getIndex(quint32 host) const;

    enum UserRoles {
        IpRole = Qt::UserRole + 1,
        NameRole,
        HostRole
    };
    //Метод синхронизации. Просматривает входящую таблицу пользователей и отбирает из них тех
    //у кого открыты оба соединения на приём и передачу
    void sync(QMap<quint32, User*> users);

protected:
    QHash<int, QByteArray> roleNames() const;
private:
    QList<UserData> m_users;
};

#endif // USERLISTMODEL_H
