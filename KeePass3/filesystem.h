#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <QObject>
#include <QVariant>
#include <QJsonObject>

#include "passwordentry.h"

class Filesystem : public QObject
{
    Q_OBJECT    

public:
    explicit Filesystem(QObject *parent = 0);
    ~Filesystem();
    Q_INVOKABLE void openFile(QString, QString);
    Q_INVOKABLE void closeFile();
    PasswordEntryModel* createModel();

Q_SIGNALS:
    void error(QString msg);
    void success(QVariantList vl);

protected:
    bool equal(char*, char*, uint);
    uint loadByte(char*, int);
    uint readHeaderField(char*, int, bool*);
    uint readBytes(char*, int, uint);    
    bool fileExists(const char *);
    std::string generateSHA256Hash(char*, uint);

private:
    QVariantList m_vl;
};

#endif // FILESYSTEM_H

