#ifndef BYTESTREAM_H
#define BYTESTREAM_H

#include <sys/types.h>
#include <vector>

class ByteStream
{
public:
    ByteStream(char* memblock);
    uint ReadByte();
    uint static ReadByte(char* memblock);
    ushort ReadShort();
    char Read();
    int GetPosition();

private:
    char* buffer;
    int pos;

};

#endif // BYTESTREAM_H
