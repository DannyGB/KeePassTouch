#include "passwordentry.h"

/*QString m_title;

PasswordEntry::PasswordEntry(QObject *parent) :
    QObject(parent)
{
}

PasswordEntry::PasswordEntry(const QString &title, const QString &password, QObject *parent) :
    QObject(parent), m_title(title), m_password(password)
{
}

QString PasswordEntry::title() const
{
    return m_title;
}

void PasswordEntry::setTitle(const QString &title)
{
    if(title != m_title) {
        m_title = title;
        emit titleChanged();
    }
}

QString PasswordEntry::password() const
{
    return m_password;
}

void PasswordEntry::setPassword(const QString &password)
{
    if(password != m_password) {
        m_password = password;
        emit passwordChanged();
    }
}*/

PasswordEntry::PasswordEntry(const QString &title, const QString &password)
    : m_title(title), m_password(password)
{
}

QString PasswordEntry::title() const
{
    return m_title;
}

QString PasswordEntry::password() const
{
    return m_password;
}

PasswordEntryModel::PasswordEntryModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

void PasswordEntryModel::addPasswordEntry(const PasswordEntry &passwordEntry)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_passwordEntries << passwordEntry;
    endInsertRows();
}

int PasswordEntryModel::rowCount(const QModelIndex & parent) const {
    Q_UNUSED(parent);
    return m_passwordEntries.count();
}

QVariant PasswordEntryModel::data(const QModelIndex & index, int role) const {
    if (index.row() < 0 || index.row() >= m_passwordEntries.count())
        return QVariant();

    const PasswordEntry &passwordEntry = m_passwordEntries[index.row()];
    if (role == TitleRole)
        return passwordEntry.title();
    else if (role == PasswordRole)
        return passwordEntry.password();
    return QVariant();
}

QHash<int, QByteArray> PasswordEntryModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[TitleRole] = "title";
    roles[PasswordRole] = "password";
    return roles;
}
