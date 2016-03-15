#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QVariant>
#include <QJsonObject>
#include <vector>

#include "treenode.h"
#include "passwordentry.h"
#include "salsa20.h"

using namespace std;

class Database : public QObject
{
    Q_OBJECT    

public:
    explicit Database(QObject *parent = 0);
    ~Database();
    Q_INVOKABLE void openFile(QString, QString, QString);
    Q_INVOKABLE void closeFile();    
    Q_INVOKABLE void selectBranch(QString);
    Q_INVOKABLE QString reloadBranch(QString, int);
    Q_INVOKABLE void search(QString);
    PasswordEntryModel* createModel();    
    Q_INVOKABLE void loadHome();
    Q_INVOKABLE void deleteFile(QString);

Q_SIGNALS:
    void error(QString msg);
    void success();

protected:
    bool equal(char*, char*, uint);
    uint loadByte(char*, int);
    uint readHeaderField(char*, int, bool*, bool*);
    uint readBytes(char*, int, uint);    
    bool fileExists(const char *);
    std::string generateSHA256Hash(char*, uint);
    bool getChildBranch(QString, vector<TreeNode*>);
    bool getMyBranch(QString, vector<TreeNode*>);
    char* readFile(QString, std::streampos&);
    void readPayload(vector<char>*, vector<char>);
    void searchInternal(QString, vector<TreeNode*>);

private:
    Salsa20* salsa;
    char *m_pbMasterSeed; //[32];
    char *m_pbCompression;
    char *m_cypherUuid; //[16];
    char *m_pbTransformSeed;//[32];
    char *m_pwDatabaseKeyEncryptionRounds;//[8];
    char *m_pbEncryptionIV;//[16];
    char *m_pbProtectedStreamKey;//[32];
    char *m_pbStreamStartBytes;//[32];
    char *m_pbInnerRandomStreamID;//[4];
};

#endif // DATABASE_H

