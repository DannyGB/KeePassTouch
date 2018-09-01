/*
* This file is part of KeePit
*
* Copyright (C) 2018 Dan Beavon
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
import Ubuntu.Components 1.3

Item {

    id: createDatabase;
    property var createDatabasePage: page_create_database;

    function checkPasswordsMatch() {
        repeat_master_password.color = master_password.text !== repeat_master_password.text ? UbuntuColors.red : UbuntuColors.darkAubergine
    }

    Page {
        id: page_create_database;
        title: i18n.tr("Create Database");

        head {
            actions: [
                Action {
                     iconName: "import"
                     text: i18n.tr("Import key")
                     onTriggered: {
                         databaseListView.setKeyMode();
                         pageStack.push(databaseListView)
                     }
                }
            ]
        }

        Flow {
            spacing: units.gu(1)
            anchors {
                margins: units.gu(2)
                fill: parent
            }

            Label {
                text: i18n.tr("Master Password")
            }

            PasswordInput {
                id: master_password
                placeholderText: i18n.tr("Master Password")
                inputWidth: parent.width
                onToggleClicked: {
                    repeat_master_password.enabled = !state
                }
                onTextChanged: checkPasswordsMatch()
            }

            PasswordInput {
                id: repeat_master_password
                toggleVisible: false
                placeholderText: i18n.tr("Repeat Password")
                inputWidth: parent.width
                onTextChanged: checkPasswordsMatch()
            }

            Label {
                text: i18n.tr("Key file")
                color: UbuntuColors.darkAubergine
            }

            TextField {
                id: keyfile
                readOnly: true
                color: UbuntuColors.darkAubergine
                width: parent.width
            }

            Button {
                id: create_keyfile
                text: i18n.tr("Create Key")
                width: parent.width
                onClicked: {
                    //Create an XML key file
                    //Save to the file application location
                }
            }

            KeySelector {
                id: combo
                width: parent.width
                onKeySelected: {
                    keyFilePath = filePath;
                    keyFileName = fileName;
                }
            }

            Button {
                id: save
                text: i18n.tr("Save")
                width: parent.width
                color: UbuntuColors.green
                onClicked: {
                    //Create an XML key file
                    //Save to the file application location
                }
            }
        }
    }
}
