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

#ifndef COMPOSITEKEY_H
#define COMPOSITEKEY_H

#include <QObject>
#include "./cryptopp/modes.h"

using namespace std;

class CompositeKey
{
public:
    CompositeKey(vector<char>, vector<char>);
    vector<char> generateKey32(vector<char>, ulong);

protected:
    vector<char> createRawCompositeKey(vector<char>);
    vector<char> transformKey(vector<char>, vector<char>, ulong);

private:
    vector<char> m_pbKey;
    vector<char> m_pbKeyFile;
    bool hasKeyFile = false;

Q_SIGNALS:

public slots:
};

#endif // COMPOSITEKEY_H
