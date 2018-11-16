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

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>
#include <qqmlcontext.h>

#include "database.h"
#include "passwordentry.h"

///
/// \brief main
/// \param argc
/// \param argv
/// \return
///
int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    Database fs;
    PasswordEntryModel* model = fs.createModel();

    qmlRegisterType<Database>("KeePass3", 1, 0, "Database");

    QQuickView view;
    view.setSource(QUrl(QStringLiteral("qrc:///qml//Main.qml")));
    view.setResizeMode(QQuickView::SizeRootObjectToView);

    QQmlContext *ctxt = view.rootContext();
    ctxt->setContextProperty("database", &fs);
    ctxt->setContextProperty("passwordEntryModel", model);
    view.show();
    return app.exec();
}
