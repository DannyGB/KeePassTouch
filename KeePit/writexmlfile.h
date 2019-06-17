/*
* This file is part of KeePit
*
* Copyright (C) 2016-2018 Dan Beavon
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

#ifndef WRITEXMLFILE_H
#define WRITEXMLFILE_H

#include "passwordentry.h"
#include "treenode.h"
#include "salsa20.h"
#include "base64.h"
#include "tinyxml2.h"
#include <string>
#include <vector>
#include <QString>

using namespace std;
using namespace tinyxml2;

class WriteXmlFile
{
public:
    WriteXmlFile(const char*, size_t, Salsa20*);
    ~WriteXmlFile();
    static QString CreateNewDatabase(QString fileName);

private:
    size_t m_size;
    Base64 base64;
    Salsa20 *m_salsa;
    const char *m_xml;
    TreeNode *lastRead = 0;
    static const char * LASTTOPVISIBLEENTRY;
    static const char * ENABLESEARCHING;
    static const char * ENABLEAUTOTYPE;
    static const char * DEFAULTAUTOTYPESEQUENCE;
    static const char * ISEXPANDED;
    static const char * LOCATIONCHANGED;
    static const char * USAGECOUNT;
    static const char * EXPIRES;
    static const char * EXPIRYTIME;
    static const char * LASTACCESSTIME;
    static const char * LASTMODIFICATIONTIME;
    static const char * CREATIONTIME;
    static const char * TIMES;
    static const char * NOTES;
    static const char * NAME;
    static const char * UUID;
    static const char * DELETEDOBJECTS;
    static const char * GROUP;
    static const char * ROOT;
    static const char * CUSTOMDATA;
    static const char * BINARIES;
    static const char * LASTSELECTEDGROUP;
    static const char * LASTTOPVISIBLEGROUP;
    static const char * HISTORYMAXSIZE;
    static const char * HISTORYMAXITEMS;
    static const char * ENTRYTEMPLATESGROUPCHANGED;
    static const char * ENTRYTEMPLATESGROUP;
    static const char * RECYCLEBINCHANGED;
    static const char * RECYCLEBINUUID;
    static const char * RECYCLEBINENABLED;
    static const char * PROTECTNOTES;
    static const char * PROTECTURL;
    static const char * PROTECTPASSWORD;
    static const char * PROTECTUSERNAME;
    static const char * PROTECTTITLE;
    static const char * MEMORYPROTECTION;
    static const char * MASTERKEYCHANGEFORCE;
    static const char * MASTERKEYCHANGEREC;
    static const char * MASTERKEYCHANGED;
    static const char * COLOR;
    static const char * MAINTENANCEHISTORYDAYS;
    static const char * DEFAULTUSERNAMECHANGED;
    static const char * DEFAULTUSERNAME;
    static const char * DATABASEDESCRIPTIONCHANGED;
    static const char * DATABASEDESCRIPTION;
    static const char * DATABASENAMECHANGED;
    static const char * DATABASENAME;
    static const char * GENERATOR;
    static const char * META;
    static const char * KEEPASSFILE;
};

#endif // WRITEXMLFILE_H
