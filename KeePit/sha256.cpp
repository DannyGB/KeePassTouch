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

#include "sha256.h"

#include "./cryptopp/modes.h"
#include "./cryptopp/sha.h"

using namespace std;

///
/// \brief SHA256::SHA256
///
SHA256::SHA256()
{
}

///
/// \brief SHA256::computeHash
/// \param message
/// \return
///
vector<char> SHA256::computeHash(vector<char> message) {

    CryptoPP::SHA256 hash;
    byte digest[ CryptoPP::SHA256::DIGESTSIZE ];

    char pb[message.size()];
    for(int i =0; i<message.size(); i++) {
        pb[i] = message[i];
    }

    hash.CalculateDigest( digest, (byte*)pb, message.size());

    vector<char> result;
    for(int i = 0; i<CryptoPP::SHA256::DIGESTSIZE;i++) {
        result.push_back(digest[i]);
    }

    return result;
}
