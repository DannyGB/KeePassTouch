#ifndef READXMLFILE_H
#define READXMLFILE_H

#include "passwordentry.h"
#include "treenode.h"
#include "salsa20.h"
#include "/home/dan/KeePass/libs/xml/tinyxml2.h"
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
    vector<TreeNode*> GetTopGroup();

protected:
    void ReadBranch(XMLElement*, vector<TreeNode*>&, TreeNode*);
    TreeNode* ReadNode(XMLElement* elem, TreeNode* parent);
    TreeNode* ExtractEntryNode(XMLElement*);
};

#endif // READXMLFILE_H
