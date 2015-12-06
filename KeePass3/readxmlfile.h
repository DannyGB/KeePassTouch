#ifndef READXMLFILE_H
#define READXMLFILE_H

#include "passwordentry.h"
#include "treenode.h"
#include <string>
#include <vector>

using namespace std;

#ifndef XMLCheckResult
    #define XMLCheckResult(a_eResult) if (a_eResult != XML_SUCCESS) { printf("Error: %i\n", a_eResult); return a_eResult; }
#endif

class ReadXmlFile
{
public:
    ReadXmlFile(const char*, size_t);
    vector<TreeNode> GetTopGroup();
};

#endif // READXMLFILE_H
