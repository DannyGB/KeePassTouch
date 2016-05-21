#include "bytestream.h"
#include "arrayextensions.h"
#include <sys/types.h>

typedef unsigned char byte;

#define BYTESIZE 4
#define SHORTSIZE 2

ByteStream::ByteStream(char* memblock, uint size)
{
    vector<char> b(memblock, memblock + size);
    buffer = b;
    pos = 0;
}

ByteStream::~ByteStream()
{
    for(uint i = 0; i<buffer.size(); i++)
    {
        buffer[i] = 0;
    }

    ArrayExtensions::Reset(buffer);
}

// Static method that reads only a byte from the given memblock array
uint ByteStream::ReadByte(char* memblock)
{
    int tmpPos = 0;

    uint result = 0;
    byte tmp[BYTESIZE];

    for(uint i = 0; i<BYTESIZE; i++) {
        tmp[i] = memblock[(i+tmpPos)];
    }

    for(int i = (BYTESIZE - 1); i>=0; i--) {
        result = (result << (BYTESIZE*2)) + (unsigned char)tmp[i];
    }

    tmpPos += BYTESIZE;

    return result;
}

// Reads a byte from the array given in the ctor and advances the pos
uint ByteStream::ReadByte()
{
    uint result = 0;
    byte tmp[BYTESIZE];

    for(uint i = 0; i<BYTESIZE; i++) {
        tmp[i] = buffer[(i+pos)];
    }

    for(int i = (BYTESIZE - 1); i>=0; i--) {
        result = (result << (BYTESIZE*2)) + (unsigned char)tmp[i];
    }

    pos += BYTESIZE;

    return result;
}

// Reads a ushort from the array given in the ctor and advances the pos
ushort ByteStream::ReadShort()
{
    ushort result = 0;
    byte tmp[SHORTSIZE];

    for(uint i = 0; i<SHORTSIZE; i++) {
        tmp[i] = buffer[(i+pos)];
    }

    for(int i = (SHORTSIZE - 1); i>=0; i--) {
        result = (result << (SHORTSIZE*2)) + (unsigned char)tmp[i];
    }

    pos += SHORTSIZE;

    return result;
}

// Reads 1 byte at a time from the array given in the ctor
char ByteStream::Read()
{
    char result = buffer[pos];
    pos += 1;

    return result;
}

// Returns the current position in the array we have read to
int ByteStream::GetPosition()
{
    return pos;
}
