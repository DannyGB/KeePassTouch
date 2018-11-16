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
    ~HashedBlockStream();
    int Read(vector<char>*, int, int);

protected:
    void Initialize(vector<char>, bool, int, bool);
    bool ReadHashedBlock();
    int readBytes(vector<char>, int, uint);

private:
    vector<char> m_pbBuffer;
    bool m_bEos = false;
    void Reset();
    bool m_bVerify;
    bool m_bWriting;
    vector<char> m_sBaseStream;
    int m_nBufferPos;
    uint m_uBufferIndex;
    int blockBufferPos;

//public slots:
};

#endif // HASHEDBLOCKSTREAM_H
