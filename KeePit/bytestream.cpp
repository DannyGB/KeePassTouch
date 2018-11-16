/*
* This file is part of KeePit
*
* Copyright (C) 2016 Dan Beavon
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "bytestream.h"
#include "arrayextensions.h"
#include <sys/types.h>

typedef unsigned char byte;

#define BYTESIZE 4
#define SHORTSIZE 2

/// \brief ByteStream::ByteStream
///        A container for a byte array
/// \param memblock The array to stream
/// \param size The size of the array
ByteStream::ByteStream(char* memblock, uint size)
{
    vector<char> b(memblock, memblock + size);
    buffer = b;
    pos = 0;
}

/// \brief ByteStream::~ByteStream
///        ByteStream destructor
ByteStream::~ByteStream()
{
    for(uint i = 0; i<buffer.size(); i++)
    {
        buffer[i] = 0;
    }

    ArrayExtensions::Reset(buffer);
}

/// \brief ByteStream::ReadByte
///        Static method that reads only a byte from the given memblock array
/// \param memblock The char array to read from
/// \return A uint containing the byte read
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

///
/// \brief ByteStream::ReadByte
///        Reads a byte from the array given in the ctor and advances the pos
/// \return
///
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

///
/// \brief ByteStream::ReadShort
///        Reads a ushort from the array given in the ctor and advances the pos
/// \return
///
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

///
/// \brief ByteStream::Read
///        Reads 1 byte at a time from the array given in the ctor
/// \return
///
char ByteStream::Read()
{
    char result = buffer[pos];
    pos += 1;

    return result;
}

///
/// \brief ByteStream::GetPosition
///        Returns the current position in the array we have read to
/// \return
///
int ByteStream::GetPosition()
{
    return pos;
}
