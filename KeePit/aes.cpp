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

#include "aes.h"
#include <string>

#include "./cryptopp/modes.h"
#include "./cryptopp/aes.h"
#include "./cryptopp/filters.h"
#include "./cryptopp/sha.h"
#include "./cryptopp/osrng.h"

using namespace std;

Aes::Aes()
{
}

string Aes::decrypt(byte * pbAesKey, uint keysize, byte* pbEncryptionIV, byte* pbFileContent, uint contentSize) {

    string recovered;
    CryptoPP::CBC_Mode< CryptoPP::AES >::Decryption d;
    d.SetKeyWithIV(pbAesKey, keysize, pbEncryptionIV );

    // The StreamTransformationFilter removes
    //  padding as required.
    CryptoPP::ArraySource ss(pbFileContent, contentSize, true,
        new CryptoPP::StreamTransformationFilter( d,
            new CryptoPP::StringSink( recovered )
        ) // StreamTransformationFilter
    ); // StringSource

    return recovered;
}
