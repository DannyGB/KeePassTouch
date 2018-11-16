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

#ifndef _BASE64_H_
#define _BASE64_H_

#include <vector>
#include <string>
//typedef unsigned char BYTE;

class Base64 {

public:
    Base64();
    std::string base64_encode(char const* buf, unsigned int bufLen);
    std::vector<char> base64_decode(std::string const&);
};
#endif
