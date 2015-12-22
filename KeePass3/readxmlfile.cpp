#include "readxmlfile.h"
#include "passwordentry.h"
#include "treenode.h"
#include "salsa20.h"
#include "base64.h"

#include <assert.h>
#include <vector>

#include "/home/dan/KeePass3/KeePass3/tinyxml2.h"

using namespace tinyxml2;

const char * m_xml;
size_t m_size;
TreeNode *lastRead = 0;
Salsa20 *m_salsa;
Base64 base64;

ReadXmlFile::ReadXmlFile(const char* xml, size_t size, Salsa20 *salsa)
{
    m_xml = xml;
    m_size = size;
    m_salsa = salsa;
}

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

    //topLevel.push_back(new TreeNode(PasswordEntry(next->GetText(), "", Group)));
    ReadBranch(next, topLevel, parent);
    return topLevel;
}

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
                       entry.title(key->NextSiblingElement("Value")->GetText());
                   }

                   if(strcmp(key->GetText(), "Password") == 0) {                       
                       vector<char> plainEncrypted = base64.base64_decode(key->NextSiblingElement("Value")->GetText());
                       if(plainEncrypted.size() > 0) {
                            byte* bytes = m_salsa->decrypt(plainEncrypted);
                            QString str;
                            for(int i=0;i<plainEncrypted.size();i++) {
                                str[i] = bytes[i];
                            }
                            entry.password(str);
                       } else {
                            entry.password(key->NextSiblingElement("Value")->GetText());
                       }
                       entry.passwordProtected(key->NextSiblingElement("Value")->Attribute("Protected"));
                   }                                     

                   if(strcmp(key->GetText(), "UserName") == 0) {
                       entry.username(key->NextSiblingElement("Value")->GetText());
                   }
               }
           }

           str = str->NextSiblingElement("String");
       }
       while(str != 0);

       node->passwordEntry(entry);
    }

    if(node != 0 && parent != 0) {
        node->parent(parent);
    }

    return node;
}
