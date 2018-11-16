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

#include "readkeyfile.h"
#include <vector>
#include <string>
#include "sha256.h"
#include "base64.h"
#include "./tinyxml2.h"

using namespace std;
using namespace tinyxml2;

///
/// \brief ReadKeyFile::ReadKeyFile
///
ReadKeyFile::ReadKeyFile()
{
}

///
/// \brief ReadKeyFile::read
/// \param memblock
/// \param size
/// \return
///
vector<char> ReadKeyFile::read(char *memblock, int size) const {

    XMLDocument doc;
    XMLError error = doc.Parse(memblock, size);

    if (error != XML_SUCCESS)
    {
        // If the file is not XML then read it directly and hash it
        SHA256 sha256;
        return sha256.computeHash(vector<char>(memblock, memblock + size));
    }

    string key;
    XMLElement* data = doc.FirstChildElement("KeyFile")->FirstChildElement("Key")->FirstChildElement("Data");

    if(data == 0) {
        throw exception();
    }

    Base64 base64;
    key = data->GetText();
    vector<char> retVal = base64.base64_decode(key);

    return retVal;
}

///
/// \brief ReadKeyFile::readHex
/// \param memblock
/// \param size
/// \return
///
vector<char> ReadKeyFile::readHex(char *memblock, int size) const {
    vector<char> retVal;

    for (int i = 0; i < size; i = i + 2)
    {
        unsigned int hex;
        sscanf(memblock + i, "%2X", &hex);
        retVal.push_back((char)hex);
    }

    return retVal;
}
