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

#include "salsa20.h"

#include "./cryptopp/modes.h"
#include "./cryptopp/salsa.h"
#include "./cryptopp/osrng.h"

using namespace std;

///
/// \brief Salsa20::Salsa20
/// \param pbKey
/// \param pbIv
///
Salsa20::Salsa20(vector<char> pbKey, char* pbIv)
{
    m_xpbKey = pbKey;
    m_IV = pbIv;
    byte key[32], iv[8];

    for(int i=0;i<32;i++) {
        key[i] = m_xpbKey[i];
    }

    for(int i=0;i<8;i++) {
        iv[i] = m_IV[i];
    }

    salsa.SetKeyWithIV(key, 32, iv);
}

///
/// \brief Salsa20::~Salsa20
///
Salsa20::~Salsa20()
{
}

///
/// \brief Salsa20::decrypt
/// \param cipherText
/// \return
///
byte* Salsa20::decrypt(vector<char> cipherText)
{
    byte* cipherTextBytes = new byte[cipherText.size()];
    byte* plainTextBytes = new byte[cipherText.size()]; // I need a way to clear this, perhaps I should return a vector of byte rather than a pointer?

    for(uint i=0;i<cipherText.size();i++) {
        cipherTextBytes[i] = cipherText[i];
    }

    salsa.ProcessData(plainTextBytes, cipherTextBytes, cipherText.size());

    delete cipherTextBytes;
    cipherTextBytes = 0;

    return plainTextBytes;
}
