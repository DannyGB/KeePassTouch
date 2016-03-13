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
};

#endif // DATABASE_H

