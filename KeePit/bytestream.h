#ifndef BYTESTREAM_H
#define BYTESTREAM_H

#include <sys/types.h>
#include <vector>

using namespace std;

class ByteStream
{
public:
    ByteStream(char* memblock, uint size);
    ~ByteStream();
    uint ReadByte();
    uint static ReadByte(char* memblock);
    ushort ReadShort();
    char Read();
    int GetPosition();

private:
    //char* buffer;
    vector<char> buffer;
    int pos;

};

#endif // BYTESTREAM_H
