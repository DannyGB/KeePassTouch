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

#ifndef SALSA20_H
#define SALSA20_H

#include "salsa20.h"
#include <QObject>
#include "./cryptopp/modes.h"
#include "./cryptopp/salsa.h"

using namespace std;

class Salsa20
{
public:
    Salsa20(vector<char> key, char* iv);
    ~Salsa20();
    byte* decrypt(vector<char>);

private:
    CryptoPP::Salsa20::Encryption salsa;
    vector<char> m_xpbKey;
    char* m_IV;

Q_SIGNALS:

public slots:
};

#endif // SALSA20_H
