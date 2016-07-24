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

import QtQuick 2.0
import QtQuick.LocalStorage 2.0
import QtGraphicalEffects 1.0
import Ubuntu.Components 1.2
import Ubuntu.Components.ListItems 1.0
import Ubuntu.Components.Popups 1.0
import Ubuntu.Components.ListItems 1.0 as ListItem
import Qt.labs.folderlistmodel 2.1

//import KeePass 1.0

Page {
    property alias pass: password
    title: i18n.tr(appTitle)
    head {
        actions: [

            Action {
                iconName: "edit-undo"
                text: i18n.tr("Close")
                onTriggered: {
                    pageStack.clear()
                    databaseListView.setDatabaseMode()
                    password.text = ''
                    keyFileName = ''
                    pageStack.push(databaseListView)
                }
            },
            Action {
                iconName: "import"
                text: i18n.tr("Import key")
                onTriggered: {
                    databaseListView.setKeyMode();
                    pageStack.push(databaseListView)
                }
            },
            Action {
                iconName: "help"
                text: i18n.tr("About")
                onTriggered: PopupUtils.open(about)
            }]
    }

    Column {
        spacing: units.gu(1)
        anchors {
            margins: units.gu(2)
            fill: parent
        }

        TextField {
            id: dbText
            width: parent.width
            text: databaseFileName
            readOnly: true
        }

        TextField {
            id: password
            placeholderText: "Enter your password"
            width: parent.width
            echoMode : TextInput.PasswordEchoOnEdit
        }

        ComboButton {
            id: combo
            width: parent.width
            text: i18n.tr("Select Key")
            onClicked: expanded = false
            ListView {
                width: parent.width
                height: 200

                FolderListModel {
                    id: folderModel
                    nameFilters: ["*.key"]
                    showDirs: false
                    folder: 'file:'+ appLocation
                }

                model: folderModel
                delegate: Standard {
                    text: fileName
                    onClicked: {
                        keyFilePath = filePath
                        keyFileName = fileName
                        combo.text = text;
                        combo.expanded = false;
                    }
                }
            }
        }

        Button {
            objectName: "button"
            width: parent.width
            color: UbuntuColors.orange
            text: i18n.tr("Open Database")

            onClicked: {
                database.openFile(databaseFilePath, password.text, keyFilePath);
            }
        }
    }
}
