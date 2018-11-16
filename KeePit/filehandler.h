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

#ifndef FILEHANDLER
#define FILEHANDLER

#include <QString>

class FileHandler
{
public:
    FileHandler();
    void static deleteFile(QString);
    char* readFile(QString, std::streampos&);
    //void openFile(QString, QString, QString);

protected:
    bool fileExists(const char*);
};

#endif // FILEHANDLER
