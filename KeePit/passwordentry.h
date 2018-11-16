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
#ifndef PASSWORDENTRY_H
#define PASSWORDENTRY_H

#include <QObject>
#include <QAbstractListModel>
#include <QStringList>

enum PasswordEntryType
{
    NotSet,
    Group,
    Entry
};

class PasswordEntry
{

public:
    PasswordEntry();
    PasswordEntry(const QString &title, const QString &password, PasswordEntryType entryType);

    QString title() const;
    void title(const QString title)
    {
        m_title = title;
    }

    QString password() const;

    void password(const QString password)
    {
        m_password = password;
    }

    bool passwordProtected() const;
    void passwordProtected(const bool passwordProtected)
    {
        m_passwordProtected = passwordProtected;
    }

    PasswordEntryType entryType() const;

    void entryType(const PasswordEntryType type) {
        m_entryType = type;
    }

    QString uuid() const;
    void uuid(const QString uuid) {
        m_uuid = uuid;
    }

    QString username() const;
    void username(const QString username) {
        m_username = username;
    }

    QString url() const;

    void url(const QString url)
    {
        m_url = url;
    }

    QString notes() const;

    void notes(const QString notes)
    {
        m_notes = notes;
    }

private:
    QString m_title;
    QString m_password;
    PasswordEntryType m_entryType;
    QString m_uuid;
    QString m_username;
    QString m_notes;
    QString m_url;
    bool m_passwordProtected;
};

class PasswordEntryModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum PasswordEntryRoles {
        TitleRole = Qt::UserRole + 1,
        PasswordRole,
        PasswordProtectedRole,
        EntryTypeRole,
        UuidRole,
        UsernameRole,
        UrlRole,
        NotesRoles
    };

    PasswordEntryModel(QObject *parent = 0);

    void addPasswordEntry(const PasswordEntry &passwordEntry);

    int rowCount(const QModelIndex & parent = QModelIndex()) const;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

    Q_INVOKABLE bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());

protected:
    QHash<int, QByteArray> roleNames() const;
private:
    QList<PasswordEntry> m_passwordEntries;
};

#endif // PASSWORDENTRY_H
