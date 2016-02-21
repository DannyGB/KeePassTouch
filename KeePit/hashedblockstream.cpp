#include "hashedblockstream.h"
#include "sha256.h"

#include <algorithm>
#include <sys/types.h>
#include <vector>
#include <stddef.h>

using namespace std;

const int m_nDefaultBufferSize = 1024 * 1024; // 1 MB
vector<char> m_sBaseStream;
bool m_bWriting;
bool m_bVerify;
bool m_bEos = false;

vector<char> m_pbBuffer;
int m_nBufferPos = 0;
uint m_uBufferIndex = 0;
int blockBufferPos = 0;

HashedBlockStream::HashedBlockStream(vector<char> sBaseStream, bool bWriting)
{
    Initialize(sBaseStream, bWriting, 0, true);
}

HashedBlockStream::HashedBlockStream(vector<char> sBaseStream, bool bWriting, int nBuffersize)
{
    Initialize(sBaseStream, bWriting, nBuffersize, true);
}

HashedBlockStream::HashedBlockStream(vector<char> sBaseStream, bool bWriting, int nBuffersize, bool bVerify)
{
    Initialize(sBaseStream, bWriting, nBuffersize, bVerify);
}

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
