#include "userlistmodel.h"

UserListModel::UserListModel(QObject *parent) : QAbstractListModel(parent)
{

}

int UserListModel::rowCount(const QModelIndex &parent) const
{
    return m_users.size();
}

QVariant UserListModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= m_users.count())
            return QVariant();
    if (role == IpRole)
        return m_users[index.row()].ip;
    else if (role == NameRole)
        return m_users[index.row()].nickname;
    else if (role == HostRole)
        return m_users[index.row()].host;
    return QVariant();
}

QVariant UserListModel::data(int index, QString role) const
{
    if (index < 0 || index >= m_users.count())
            return QVariant();
    if (role == "ip")
        return m_users[index].ip;
    else if (role == "name")
        return m_users[index].nickname;
    else if (role == "host")
        return m_users[index].host;
    return QVariant();
}

QVariant UserListModel::getIndex(quint32 host) const
{
    for(int i = 0; i < m_users.size(); i++)
        if(host == m_users[i].host)
            return i;
    return -1;
}

void UserListModel::sync(QMap<quint32, User *> users)
{
    for (auto it = users.begin(); it != users.end();){
        //если открыты оба соединения
        if(it.value()->inReady && it.value()->outReady){
            bool needAdd = true;
            //то смотрим, есть ли он уже в списке
            for(int i = 0; i < m_users.size(); i++){
                if(it.key() == m_users[i].host){
                    //Если да, то возможно у него новое имя
                    if(it.value()->nickname != m_users[i].nickname){
                        //beginInsertRows(QModelIndex(), i, i);
                        beginResetModel();
                        m_users[i].nickname = it.value()->nickname;
                        //endInsertRows();
                        endResetModel();
                    }
                    needAdd = false;
                }
            }
            //Если нет, то добавляем его
            if(needAdd){
                beginInsertRows(QModelIndex(), rowCount(), rowCount());
                m_users.append(UserData(it.value()->host,it.value()->nickname));
                endInsertRows();
            }
        }
        ++it;
    }
    //Проверяем, есть ли те, кто стал оффлайн
    for(int i = 0; i < m_users.size(); i++){
        if(!users.contains(m_users[i].host) || !users[m_users[i].host]->inReady || !users[m_users[i].host]->outReady){
            beginRemoveRows(QModelIndex(), i, i);
            m_users.removeAt(i);
            endRemoveRows();
        }
    }

}

QHash<int, QByteArray> UserListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[IpRole] = "ip";
    roles[NameRole] = "name";
    roles[HostRole] = "host";
    return roles;
}
