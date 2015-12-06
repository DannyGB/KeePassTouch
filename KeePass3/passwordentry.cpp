#include "passwordentry.h"

PasswordEntry::PasswordEntry()
{
}

PasswordEntry::PasswordEntry(const QString &title, const QString &password, PasswordEntryType entryType)
    : m_title(title), m_password(password), m_entryType(entryType)
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
