/*
* This file is part of KeePit
*
* Copyright (C) 2016 Dan Beavon
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include "hashedblockstream.h"
#include "arrayextensions.h"
#include "sha256.h"

#include <algorithm>
#include <sys/types.h>
#include <vector>
#include <stddef.h>

using namespace std;

const int m_nDefaultBufferSize = 1024 * 1024; // 1 MB

///
/// \brief HashedBlockStream::~HashedBlockStream
///
HashedBlockStream::~HashedBlockStream()
{
    this->Reset();
}

///
/// \brief HashedBlockStream::HashedBlockStream
/// \param sBaseStream
/// \param bWriting
///
HashedBlockStream::HashedBlockStream(vector<char> sBaseStream, bool bWriting)
{
    Initialize(sBaseStream, bWriting, 0, true);
}

///
/// \brief HashedBlockStream::HashedBlockStream
/// \param sBaseStream
/// \param bWriting
/// \param nBuffersize
///
HashedBlockStream::HashedBlockStream(vector<char> sBaseStream, bool bWriting, int nBuffersize)
{
    Initialize(sBaseStream, bWriting, nBuffersize, true);
}

///
/// \brief HashedBlockStream::HashedBlockStream
/// \param sBaseStream
/// \param bWriting
/// \param nBuffersize
/// \param bVerify
///
HashedBlockStream::HashedBlockStream(vector<char> sBaseStream, bool bWriting, int nBuffersize, bool bVerify)
{
    Initialize(sBaseStream, bWriting, nBuffersize, bVerify);
}

///
/// \brief HashedBlockStream::Initialize
/// \param sBaseStream
/// \param bWriting
/// \param nBufferSize
/// \param bVerify
///
void HashedBlockStream::Initialize(vector<char> sBaseStream, bool bWriting, int nBufferSize, bool bVerify) {
    //if(sBaseStream == NULL) throw exception();
    if(nBufferSize < 0) throw exception();

    if(nBufferSize == 0) nBufferSize = m_nDefaultBufferSize;

    m_nBufferPos = 0;
    m_uBufferIndex = 0;
    blockBufferPos = 0;
    m_sBaseStream = sBaseStream;
    m_bWriting = bWriting;
    m_bVerify = bVerify;
}

///
/// \brief HashedBlockStream::Read
/// \param pbBuffer
/// \param nOffset
/// \param nCount
/// \return
///
int HashedBlockStream::Read(vector<char>* pbBuffer, int nOffset, int nCount)
{
    if(m_bWriting) throw exception();

    int nRemaining = nCount;
    while(nRemaining > 0)
    {
        if(m_nBufferPos == (int)m_pbBuffer.size())
        {
            if(ReadHashedBlock() == false)
                return nCount - nRemaining; // Bytes actually read
        }

        int nCopy = min((int)m_pbBuffer.size() - m_nBufferPos, nRemaining);

        for(int i=m_nBufferPos;i<(nCopy + m_nBufferPos);i++) {
            pbBuffer->push_back(m_pbBuffer[i]);
        }

        nOffset += nCopy;
        m_nBufferPos += nCopy;

        if(nCopy < nRemaining) {
            return nCopy;
        }

        nRemaining -= nCopy;
    }

    return nCount;
}

///
/// \brief HashedBlockStream::ReadHashedBlock
/// \return
///
bool HashedBlockStream::ReadHashedBlock()
{
    if(m_bEos) return false; // End of stream reached already

    m_nBufferPos = 0;

    blockBufferPos = max(blockBufferPos, 0);
    // Reads the block ID
    uint blockId = readBytes(m_sBaseStream, blockBufferPos, 4);
    if(blockId != m_uBufferIndex)
    {
        throw exception();
    }

    blockBufferPos += 4;

    ++m_uBufferIndex;
    vector<char> pbStoredHash;

    // Read the block hash
    for(uint i=0;i<32;i++) {
       pbStoredHash.push_back(m_sBaseStream[blockBufferPos++]);
    }

    // Advance the position of the buffer position by the number of bytes read to get the block hash
    if(pbStoredHash.size() != 32)
        throw new exception();

    // Read the Data size
    int nBufferSize = 0;
    nBufferSize = readBytes(m_sBaseStream, blockBufferPos, 4);
    blockBufferPos += 4;

    if(nBufferSize < 0)
        throw exception();

    if(nBufferSize == 0)
    {
        for(int iHash = 0; iHash < 32; ++iHash)
        {
            if(pbStoredHash[iHash] != 0)
                throw exception();
        }

        m_bEos = true;
        //m_pbBuffer((int)m_pbBuffer.size());
        return false;
    }

    // Read the block data
    int i=0;
    m_pbBuffer.clear();
    while(i<(nBufferSize)) {
        m_pbBuffer.push_back(m_sBaseStream[(i+blockBufferPos)]);
        i++;
    }

    blockBufferPos += nBufferSize;

    if((int)m_pbBuffer.size() != nBufferSize && m_bVerify) {
        throw new exception();
    }

    if(m_bVerify)
    {
        SHA256 sha256;
        vector<char> pbComputedHash = sha256.computeHash(m_pbBuffer);
        if(pbComputedHash.size()!= 32)
            throw exception();

        for(int iHashPos = 0; iHashPos < 32; ++iHashPos)
        {
            if(pbStoredHash[iHashPos] != pbComputedHash[iHashPos])
                throw exception();
        }
    }

    return true;
}

///
/// \brief HashedBlockStream::readBytes
/// \param memblock
/// \param offset
/// \param size
/// \return
///
int HashedBlockStream::readBytes(vector<char> memblock, int offset, uint size)
{
    int result = 0;
    char tmp[size];

    for(uint i = 0; i<size; i++) {
        tmp[i] = memblock[(i+offset)];
    }

    for(int i = (size - 1); i>=0; i--) {
        result = (result << 8) + (unsigned char)tmp[i];
    }

    return result;
}

///
/// \brief HashedBlockStream::Reset
///
void HashedBlockStream::Reset()
{
    for(uint i = 0; i<m_pbBuffer.size() ;i++) {
        m_pbBuffer[i] = 0;
    }

    ArrayExtensions::Reset(m_pbBuffer);

    for(uint i = 0; i<m_sBaseStream.size() ;i++) {
        m_sBaseStream[i] = 0;
    }

    ArrayExtensions::Reset(m_sBaseStream);
}
