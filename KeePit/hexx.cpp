/*
* This file is part of KeePit
*
* Copyright (C) 2016-2018 Dan Beavon
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

#include "hexx.h"

QString Hex::ByteArrayToHexString(QByteArray array)
{
    int nLen = array.size();
    if(nLen == 0) return QString();

    QString sb;

    unsigned char bt;
    unsigned char btHigh;
    unsigned char btLow;

    for(int i = 0; i < nLen; ++i)
    {
        bt = array[i];
        btHigh = bt; btHigh >>= 4;
        btLow = (unsigned char)(bt & 0x0F);

        if(btHigh >= 10) sb.append((char)('A' + btHigh - 10));
        else sb.append((char)('0' + btHigh));

        if(btLow >= 10) sb.append((char)('A' + btLow - 10));
        else sb.append((char)('0' + btLow));
    }

    return sb;
}

QByteArray Hex::HexStringToByteArray(QString strHex)
{
    std::vector<char> bytes;
    std::string hex = strHex.toStdString();

    for (unsigned int i = 0; i < hex.length(); i += 2) {
        std::string byteString = hex.substr(i, 2);
        char byte = (char) strtol(byteString.c_str(), NULL, 16);
        bytes.push_back(byte);
    }

    QByteArray qb(reinterpret_cast<const char*>(bytes.data()), bytes.size());
    return qb;
}