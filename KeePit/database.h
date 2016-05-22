/*
* This file is part of KeePit
*
* Copyright (C) 2016 Dan Beavon
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QVariant>
#include <QJsonObject>
#include <vector>

#include "treenode.h"
#include "passwordentry.h"
#include "salsa20.h"
#include "bytestream.h"

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
    void readHeaderField(ByteStream*, bool*, bool*);
    uint readBytes(char*, int, uint);    
    bool fileExists(const char *);
    std::string generateSHA256Hash(char*, uint);
    bool getChildBranch(QString, vector<TreeNode*>);
    bool getMyBranch(QString, vector<TreeNode*>);    
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
    bool foundAny = false;
    vector<TreeNode*> dataTree;
    vector<TreeNode*> current;
    uint uCompression;
};

#endif // DATABASE_H

