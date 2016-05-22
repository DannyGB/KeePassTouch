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

#ifndef ARRAYEXTENSIONS_H
#define ARRAYEXTENSIONS_H

#include <sys/types.h>
#include <vector>

using namespace std;

class ArrayExtensions
{
public:
    ArrayExtensions();
    vector<char> toVector(char*, unsigned int);

    bool static Equal(char* t1, char* t2, uint size)
    {
        for(uint i = 0; i<size;i++) {
            if(t1[i] != t2[i]) {
                return false;
            }
        }

        return true;
    }

    void static Reset(vector<char> &v)
    {        
        /* Overwrite all entries with 0 then deallocate */
        for(uint i = 0; i<v.size(); i++)
        {
            v[i] = 0;
        }

        vector<char>().swap(v);
    }
};

#endif // ARRAYEXTENSIONS_H
