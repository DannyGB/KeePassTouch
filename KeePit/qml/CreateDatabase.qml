/*
* This file is part of KeePit
*
* Copyright (C) 2018 Dan Beavon
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

import QtQuick 2.4
import Ubuntu.Components 1.3
import QtQuick.Layouts 1.1

Item {

    id: createDatabase;
    property var createDatabasePage: page_create_database;

    function checkPasswordsMatch() {
        repeat_master_password.color = master_password.text !== repeat_master_password.text ? UbuntuColors.red : UbuntuColors.darkAubergine
    }

    Page {
        id: page_create_database;
        header: PageHeader {
            id: pageHeader
            title: i18n.tr("Create Database");
            trailingActionBar {
                actions: [
                    Action {
                        iconName: "import"
                        onTriggered: {
                            pageStack.push(importer.pickerPage)
                        }
                    }
                ]
            }
        }

        Flow {
            spacing: units.gu(1)
            anchors {
                margins: units.gu(2)
                fill: parent
                topMargin: pageHeader.height + units.gu(2)
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
                labelText: i18n.tr("Use Existing Key")
                onKeySelected: {
                    keyFilePath = filePath;
                    keyFileName = fileName;
                }
                onToggleChecked: {
                    if(!state) {
                        keyFilePath = "";
                        keyFileName = "";
                    }
                }
            }

            /*Button {
                id: save
                text: i18n.tr("Save")
                width: parent.width
                color: UbuntuColors.green
                onClicked: {
                    //Create an XML key file
                    //Save to the file application location
                }
            }*/
        }
    }
}
