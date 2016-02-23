#include "userlistmodel.h"

UserListModel::UserListModel(QObject *parent) : QAbstractListModel(parent)
{
    m_users.append(UserData(QHostAddress(), "me"));
    m_users.append(UserData(QHostAddress(), "you"));
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

void UserListModel::sync(QMap<quint32, User *> users)
{
    for (auto it = users.begin(); it != users.end();){
        if(it.value()->ready){
            bool needAdd = true;
            for(int i = 0; i < m_users.size(); i++){
                if(it.key() == m_users[i].host){
                    if(it.value()->nickname != m_users[i].nickname){
                        beginInsertRows(QModelIndex(), i, i+1);
                        m_users[i].nickname = it.value()->nickname;
                        endInsertRows();
                    }
                    needAdd = false;
                }
            }
            if(needAdd){
                beginInsertRows(QModelIndex(), rowCount(), rowCount());
                m_users.append(UserData(it.value()->host,it.value()->nickname));
                endInsertRows();
            }
        }
        ++it;
    }

    for(int i = 0; i < m_users.size(); i++){
        if(!users.contains(m_users[i].host) || !users[m_users[i].host]->ready){
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
