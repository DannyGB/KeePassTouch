#include "readkeyfile.h"
#include <vector>
#include <string>

#include "base64.h"
#include "/home/dan/KeePass3/KeePass3/tinyxml2.h"

using namespace std;
using namespace tinyxml2;

ReadKeyFile::ReadKeyFile()
{
}

vector<char> ReadKeyFile::read(char *memblock, int size) const {

    XMLDocument doc;
    doc.Parse(memblock, size);

    string key;
    XMLElement* data = doc.FirstChildElement("KeyFile")->FirstChildElement("Key")->FirstChildElement("Data");

    if(data == 0) {
        throw exception();
    }

    Base64 base64;
    key = data->GetText();
    vector<char> retVal = base64.base64_decode(key);

    return retVal;
}
