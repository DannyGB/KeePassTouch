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

#include "arrayextensions.h"

ArrayExtensions::ArrayExtensions()
{
}

/// \brief ArrayExtensions::toVector
///        Converts a pointer to a char array into a Vector
/// \param source The source pointer to a char array
/// \param size The size of the array
/// \return A vector containing the contents of the char array
vector<char> ArrayExtensions::toVector(char* source, unsigned int size)
{
    vector<char> destination;
    for(unsigned int i = 0; i<size;i++) {
        destination.push_back(source[i]);
    }

    return destination;
}
