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
    //QString ToString(vector<TreeNode*>);
    static void CreateNewDatabase(QString fileName);

private:
    size_t m_size;
    Base64 base64;
    Salsa20 *m_salsa;
    const char * m_xml;
    TreeNode *lastRead = 0;
};

#endif // WRITEXMLFILE_H
