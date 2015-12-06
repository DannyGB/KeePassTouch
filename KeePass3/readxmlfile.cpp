#include "readxmlfile.h"
#include "passwordentry.h"
//#include "passwordentrymodel.h"

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

//vector<const char *> ReadXmlFile::GetTopGroup()
QList<QObject*> ReadXmlFile::GetTopGroup()
{
    XMLDocument doc;
    doc.Parse(m_xml, m_size);

    // Need to return a list of something (at least UUID and name but perhaps the whole thing)
    // This we then push up to QT for display in a list


    //PasswordEntryModel model;
    //vector<const char *> names;
    QList<QObject*> entries;
    XMLElement* keePassFile = doc.FirstChildElement("KeePassFile");

    if(keePassFile == 0) {
        throw exception();
    }

    XMLElement* root = keePassFile->FirstChildElement("Root");
    XMLElement* group = root->FirstChildElement("Group");

    XMLElement* next = group->FirstChildElement("Name");
    //model.addPasswordEntry(PasswordEntry(next->GetText(), "testing"));
    //names.push_back(next->GetText());
    //PasswordEntry* password;
    //password->title(next->GetText());
    //entries.push_back(new PasswordEntry(next->GetText(), "test1"));

    do {
        next = next->NextSiblingElement();
        if(next != 0) {
            if(strcmp(next->Name(), "Entry") == 0) {
                XMLElement* str = next->FirstChildElement("String");
                do {
                    if(str != 0) {
                        XMLElement* key = str->FirstChildElement("Key");
                        if(key != 0) {
                            if(strcmp(key->GetText(), "Title") == 0) {
                               // names.push_back(key->NextSiblingElement("Value")->GetText());
                                //PasswordEntry* password;
                                //password->title(next->GetText());
                                //entries.push_back(new PasswordEntry(key->NextSiblingElement("Value")->GetText(), "test1"));
                               // model.addPasswordEntry(PasswordEntry(key->NextSiblingElement("Value")->GetText(), "testing"));
                            }
                        }
                    }

                    str = str->NextSiblingElement("String");
                }
                while(str != 0);
            }
        }
    }
    while(next != 0);

   /* QQuickView view;
       view.setResizeMode(QQuickView::SizeRootObjectToView);
       QQmlContext *ctxt = view.rootContext();
       ctxt->setContextProperty("passwordEntryModel", &model);*/

    return entries;
}

vector<string> ReadXmlFile::GetGroupAt(const char* UUID)
{

}

