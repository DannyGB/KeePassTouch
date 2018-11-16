/*
* This file is part of KeePit
*
* Copyright (C) 2016 Dan Beavon
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "passwordentry.h"

///
/// \brief PasswordEntry::PasswordEntry
///
PasswordEntry::PasswordEntry()
{
}

///
/// \brief PasswordEntry::PasswordEntry
/// \param title
/// \param password
/// \param entryType
///
PasswordEntry::PasswordEntry(const QString &title, const QString &password, PasswordEntryType entryType)
    : m_title(title), m_password(password), m_entryType(entryType)
{
}

///
/// \brief PasswordEntry::title
/// \return
///
QString PasswordEntry::title() const
{
    return m_title;
}

///
/// \brief PasswordEntry::password
/// \return
///
QString PasswordEntry::password() const
{
    return m_password;
}

///
/// \brief PasswordEntry::passwordProtected
/// \return
///
bool PasswordEntry::passwordProtected() const
{
    return m_passwordProtected;
}

///
/// \brief PasswordEntry::entryType
/// \return
///
PasswordEntryType PasswordEntry::entryType() const
{
    return m_entryType;
}

///
/// \brief PasswordEntry::uuid
/// \return
///
QString PasswordEntry::uuid() const
{
    return m_uuid;
}

///
/// \brief PasswordEntry::username
/// \return
///
QString PasswordEntry::username() const
{
    return m_username;
}

///
/// \brief PasswordEntry::url
/// \return
///
QString PasswordEntry::url() const
{
    return m_url;
}

///
/// \brief PasswordEntry::notes
/// \return
///
QString PasswordEntry::notes() const
{
    return m_notes;
}

///
/// \brief PasswordEntryModel::PasswordEntryModel
/// \param parent
///
PasswordEntryModel::PasswordEntryModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

///
/// \brief PasswordEntryModel::addPasswordEntry
/// \param passwordEntry
///
void PasswordEntryModel::addPasswordEntry(const PasswordEntry &passwordEntry)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_passwordEntries << passwordEntry;
    endInsertRows();
}

///
/// \brief PasswordEntryModel::rowCount
/// \param parent
/// \return
///
int PasswordEntryModel::rowCount(const QModelIndex & parent) const {
    Q_UNUSED(parent);
    return m_passwordEntries.count();
}

///
/// \brief PasswordEntryModel::data
/// \param index
/// \param role
/// \return
///
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

///
/// \brief PasswordEntryModel::roleNames
/// \return
///
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

///
/// \brief PasswordEntryModel::removeRows
/// \param row
/// \param count
/// \param parent
/// \return
///
bool PasswordEntryModel::removeRows(int row, int count, const QModelIndex &parent) {
        Q_UNUSED(parent);
        beginRemoveRows(QModelIndex(), row, row + count - 1);
        while (count--) m_passwordEntries.takeAt(row);
        endRemoveRows();
        return true;
}
