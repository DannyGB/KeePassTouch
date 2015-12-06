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

class PasswordEntry
{
public:
    PasswordEntry(const QString &title, const QString &password);

    QString title() const;
    QString password() const;

private:
    QString m_title;
    QString m_password;
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
