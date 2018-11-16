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

/// \brief Aes::decrypt
///        Decrypts an AES byte array using CryptoPP
/// \param pbAesKey The Aes key used to decrypt the file
/// \param keysize The size of the key
/// \param pbEncryptionIV The encryption IV
/// \param pbFileContent The pointer to a byte array containing the Aes encrypted content
/// \param contentSize The size of the content
/// \return A string containing the decrypted file content
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
