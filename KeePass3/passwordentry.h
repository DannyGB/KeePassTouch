#ifndef PASSWORDENTRY_H
#define PASSWORDENTRY_H

#include <QObject>
#include <QAbstractListModel>
#include <QStringList>

/*class PasswordEntry : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString password READ password WRITE setPassword NOTIFY passwordChanged)

public:

    PasswordEntry(QObject *parent = 0);
    PasswordEntry(const QString &title, const QString &password, QObject *parent = 0);

    QString title() const;
    void setTitle(const QString &title);

    QString password() const;
    void setPassword(const QString &password);

signals:
    void titleChanged();
    void passwordChanged();

private:
    QString m_title;
    QString m_password;
};*/

enum PasswordEntryType : int
{
    NotSet,
    Group,
    Password
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

    PasswordEntryType entryType();

private:
    QString m_title;
    QString m_password;
    PasswordEntryType m_entryType;
};

class PasswordEntryModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum PasswordEntryRoles {
        TitleRole = Qt::UserRole + 1,
        PasswordRole
    };

    PasswordEntryModel(QObject *parent = 0);

    void addPasswordEntry(const PasswordEntry &passwordEntry);

    int rowCount(const QModelIndex & parent = QModelIndex()) const;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

protected:
    QHash<int, QByteArray> roleNames() const;
private:
    QList<PasswordEntry> m_passwordEntries;
};

#endif // PASSWORDENTRY_H
