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

bool PasswordEntry::passwordProtected() const
{
    return m_passwordProtected;
}

PasswordEntryType PasswordEntry::entryType() const
{
    return m_entryType;
}

QString PasswordEntry::uuid() const
{
    return m_uuid;
}

QString PasswordEntry::username() const
{
    return m_username;
}

QString PasswordEntry::url() const
{
    return m_url;
}

QString PasswordEntry::notes() const
{
    return m_notes;
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
    else if(role == EntryTypeRole)
        return passwordEntry.entryType();
    else if(role == UuidRole)
        return passwordEntry.uuid();
    else if(role == UsernameRole)
        return passwordEntry.username();
    else if(role == PasswordProtectedRole)
        return passwordEntry.passwordProtected();
    else if(role == NotesRoles)
        return passwordEntry.notes();
    else if(role == UrlRole)
        return passwordEntry.url();
    return QVariant();
}

QHash<int, QByteArray> PasswordEntryModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[TitleRole] = "title";
    roles[PasswordRole] = "password";
    roles[EntryTypeRole] = "entryType";
    roles[UuidRole] = "uuid";
    roles[UsernameRole] = "username";
    roles[PasswordProtectedRole] = "passwordProtected";
    roles[UrlRole] = "url";
    roles[NotesRoles] = "notes";
    return roles;
}

bool PasswordEntryModel::removeRows(int row, int count, const QModelIndex &parent) {
        Q_UNUSED(parent);
        beginRemoveRows(QModelIndex(), row, row + count - 1);
        while (count--) m_passwordEntries.takeAt(row);
        endRemoveRows();
        return true;
    }
