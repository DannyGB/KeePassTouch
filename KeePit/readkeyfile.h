#ifndef READKEYFILE_H
#define READKEYFILE_H

#include <string>
#include <vector>

using namespace std;

class ReadKeyFile
{
public:
    ReadKeyFile();
    vector<char> read(char*, int) const;
};

#endif // READKEYFILE_H
