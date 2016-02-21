#ifndef HASHEDBLOCKSTREAM_H
#define HASHEDBLOCKSTREAM_H

#include <sys/types.h>
#include <vector>

using namespace std;

class HashedBlockStream
{
public:
    HashedBlockStream(vector<char>, bool);
    HashedBlockStream(vector<char>, bool, int);
    HashedBlockStream(vector<char>, bool, int, bool);
    int Read(vector<char>*, int, int);

protected:
    void Initialize(vector<char>, bool, int, bool);
    bool ReadHashedBlock();
    int readBytes(vector<char>, int, uint);

//public slots:
};

#endif // HASHEDBLOCKSTREAM_H
