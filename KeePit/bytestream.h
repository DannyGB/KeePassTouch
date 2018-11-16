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
