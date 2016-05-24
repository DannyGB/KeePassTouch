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

#ifndef READXMLFILE_H
#define READXMLFILE_H

#include "passwordentry.h"
#include "treenode.h"
#include "salsa20.h"
#include "base64.h"
#include "tinyxml2.h"
#include <string>
#include <vector>

using namespace std;
using namespace tinyxml2;

#ifndef XMLCheckResult
    #define XMLCheckResult(a_eResult) if (a_eResult != XML_SUCCESS) { printf("Error: %i\n", a_eResult); return a_eResult; }
#endif

class ReadXmlFile
{
public:
    ReadXmlFile(const char*, size_t, Salsa20*);
    ~ReadXmlFile();
    vector<TreeNode*> GetTopGroup();

protected:
    void ReadBranch(XMLElement*, vector<TreeNode*>&, TreeNode*);
    TreeNode* ReadNode(XMLElement* elem, TreeNode* parent);
    TreeNode* ExtractEntryNode(XMLElement*);

private:
    size_t m_size;
    Base64 base64;
    Salsa20 *m_salsa;
    const char * m_xml;
    TreeNode *lastRead = 0;
};

#endif // READXMLFILE_H
