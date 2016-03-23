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

//#include <QtQml>
//#include <QtQml/QQmlContext>
#include "backend.h"
#include "database.h"
//#include "passwordentrymodel.h"
#include "passwordentry.h"


void BackendPlugin::registerTypes(const char *uri)
{
    Q_ASSERT(uri == QLatin1String("KeePass"));

    //qmlRegisterType<Filesystem>(uri, 1, 0, "Filesystem");
    //qmlRegisterType<PasswordEntryModel>(uri, 1, 0, "PasswordEntryModel");
    //qmlRegisterType<PasswordEntry>(uri, 1, 0, "PasswordEntry");
}

void BackendPlugin::initializeEngine(QQmlEngine *engine, const char *uri)
{
    QQmlExtensionPlugin::initializeEngine(engine, uri);
}

