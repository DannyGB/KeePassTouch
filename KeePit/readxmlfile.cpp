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
#include "readxmlfile.h"
#include "passwordentry.h"
#include "treenode.h"
#include "salsa20.h"
#include "base64.h"

#include <assert.h>
#include <vector>

#include "./tinyxml2.h"

using namespace tinyxml2;

///
/// \brief ReadXmlFile::ReadXmlFile
/// \param xml
/// \param size
/// \param salsa
///
ReadXmlFile::ReadXmlFile(const char* xml, size_t size, Salsa20 *salsa)
{
    m_xml = xml;
    m_size = size;
    m_salsa = salsa;
}

///
/// \brief ReadXmlFile::~ReadXmlFile
///
ReadXmlFile::~ReadXmlFile()
{
    delete m_salsa;
    m_salsa = 0;
    delete lastRead;
    lastRead = 0;
}

///
/// \brief ReadXmlFile::GetTopGroup
/// \return
///
vector<TreeNode*> ReadXmlFile::GetTopGroup()
{
    XMLDocument doc;
    doc.Parse(m_xml, m_size);

    vector<TreeNode*> topLevel;
    TreeNode* parent = 0;
    XMLElement* keePassFile = doc.FirstChildElement("KeePassFile");

    if(keePassFile == 0) {
        throw exception();
    }

    XMLElement* root = keePassFile->FirstChildElement("Root");
    XMLElement* group = root->FirstChildElement("Group");
    XMLElement* next = group->FirstChildElement("Name");

    ReadBranch(next, topLevel, parent);
    return topLevel;
}

///
/// \brief ReadXmlFile::ReadBranch
/// \param node
/// \param current
/// \param parent
///
void ReadXmlFile::ReadBranch(XMLElement* node, vector<TreeNode*> &current, TreeNode *parent)
{
    do {
       node = node->NextSiblingElement();
       if(node != 0) {
           TreeNode* tn = ReadNode(node, parent);
           if(tn != 0) {
                current.push_back(tn);
           }
       }
    }
    while(node != 0);
}

///
/// \brief ReadXmlFile::ReadNode
/// \param elem
/// \param parent
/// \return
///
TreeNode* ReadXmlFile::ReadNode(XMLElement* elem, TreeNode *parent)
{
    TreeNode* node = 0;
    if(strcmp(elem->Name(), "Group") == 0) {
        node = new TreeNode();
        PasswordEntry entry;
        entry.entryType(Group);
        entry.title(elem->FirstChildElement("Name")->GetText());
        entry.entryType(Group);
        entry.uuid(elem->FirstChildElement("UUID")->GetText());
        node->passwordEntry(entry);
        vector<TreeNode*> b;
        ReadBranch(elem->FirstChildElement(), b, node);
        node->next(b);
    }
    else if(strcmp(elem->Name(), "Entry") == 0) {
        node = new TreeNode();
        node = ExtractEntryNode(elem);
    }

    if(node != 0 && parent != 0) {
        node->parent(parent);
    }

    return node;
}

///
/// \brief ReadXmlFile::ExtractEntryNode
/// \param elem
/// \return
///
TreeNode* ReadXmlFile::ExtractEntryNode(XMLElement* elem)
{
    TreeNode* node = new TreeNode();

    PasswordEntry entry;
    entry.entryType(Entry);
    XMLElement* uuid = elem->FirstChildElement("UUID");
    if(uuid != 0) {
        entry.uuid(uuid->GetText());
    }

    XMLElement* str = elem->FirstChildElement("String");
    do {
        if(str != 0) {
            XMLElement* key = str->FirstChildElement("Key");
            if(key != 0) {
                if(strcmp(key->GetText(), "Title") == 0) {
                    const char* t = key->NextSiblingElement("Value")->GetText();
                    if(t != 0) {
                     entry.title(t);
                    }
                }

                else if(strcmp(key->GetText(), "Password") == 0) {
                    const char* p = key->NextSiblingElement("Value")->GetText();
                    if(p != 0){
                        std::string strt(p);
                         vector<char> plainEncrypted = base64.base64_decode(strt);
                         byte* bytes = m_salsa->decrypt(plainEncrypted);
                         QString str;
                         for(uint i=0;i<plainEncrypted.size();i++) {
                             str[i] = bytes[i];
                         }
                         entry.password(str);
                    } else {
                         entry.password(key->NextSiblingElement("Value")->GetText());
                    }
                    entry.passwordProtected(key->NextSiblingElement("Value")->Attribute("Protected"));
                }

                else if(strcmp(key->GetText(), "UserName") == 0) {
                    entry.username(key->NextSiblingElement("Value")->GetText());
                }

                else if(strcmp(key->GetText(), "URL") == 0) {
                    entry.url(key->NextSiblingElement("Value")->GetText());
                }

                else if(strcmp(key->GetText(), "Notes") == 0) {
                    entry.notes(key->NextSiblingElement("Value")->GetText());
				}
                else {
                    // This case covers all other (potentially encrypted) fields.
                    // For the moment, just check whether the field is encrypted,
                    // and if it is, decrypt it.
                    const char* p = key->NextSiblingElement("Value")->GetText();
                    if(p != 0){
                        std::string strt(p);
                         vector<char> plainEncrypted = base64.base64_decode(strt);
                         byte* bytes = m_salsa->decrypt(plainEncrypted);
                    }
                    entry.passwordProtected(key->NextSiblingElement("Value")->Attribute("Protected"));
				
				}
            }

            assert(str != 0);
            str = str->NextSiblingElement("String");
        }
    }
    while(str != 0);

    XMLElement* hist = elem->FirstChildElement("History");
    if(hist != 0 && !hist->NoChildren()) {
        XMLElement* histEnt = hist->FirstChildElement("Entry");
        do {
            ExtractEntryNode(histEnt); // Discard History items for now we only need to decrypt them in order
            histEnt = histEnt->NextSiblingElement("Entry");
        } while(histEnt != 0);
    }

    node->passwordEntry(entry);

    return node;
}
