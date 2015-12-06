#include "readxmlfile.h"
#include "passwordentry.h"
#include "treenode.h"

#include <assert.h>
#include <vector>

#include "/home/dan/KeePass/libs/xml/tinyxml2.h"

//#include <qqmlcontext.h>
//#include <QtQuick/qquickview.h>

using namespace tinyxml2;

const char * m_xml;
size_t m_size;

ReadXmlFile::ReadXmlFile(const char* xml, size_t size)
{
    m_xml = xml;
    m_size = size;
}

vector<TreeNode> ReadXmlFile::GetTopGroup()
{
    XMLDocument doc;
    doc.Parse(m_xml, m_size);

    // Need to return a list of something (at least UUID and name but perhaps the whole thing)
    // This we then push up to QT for display in a list
    vector<TreeNode> names;
    XMLElement* keePassFile = doc.FirstChildElement("KeePassFile");

    if(keePassFile == 0) {
        throw exception();
    }

    XMLElement* root = keePassFile->FirstChildElement("Root");
    XMLElement* group = root->FirstChildElement("Group");
    XMLElement* next = group->FirstChildElement("Name");

    names.push_back(TreeNode(PasswordEntry(next->GetText(), "", Group), PasswordEntry("", "", NotSet), PasswordEntry("", "", NotSet)));

    // All entries and sub groups in the given group, this will need to be recursive
    do {
        next = next->NextSiblingElement();
        if(next != 0) {
            if(strcmp(next->Name(), "Entry") == 0) {
                TreeNode node;
                PasswordEntry entry;
                XMLElement* str = next->FirstChildElement("String");
                do {
                    if(str != 0) {
                        XMLElement* key = str->FirstChildElement("Key");
                        if(key != 0) {
                            if(strcmp(key->GetText(), "Title") == 0) {
                                entry.title(key->NextSiblingElement("Value")->GetText());
                                //names.push_back(TreeNode(PasswordEntry(key->NextSiblingElement("Value")->GetText(), "", Password), PasswordEntry("", "", Password), PasswordEntry("", "", Password)));
                            }

                            if(strcmp(key->GetText(), "Password") == 0) {
                                entry.password(key->NextSiblingElement("Value")->GetText());
                            }
                        }
                    }

                    str = str->NextSiblingElement("String");
                }
                while(str != 0);

                // need to keep track of the previous and next nodes (they should also be pointers)
                node.passwordEntry(entry);
                names.push_back(node);
            }
        }        
    }
    while(next != 0);

    return names;
}
