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

import QtQuick 2.4
import QtQuick.LocalStorage 2.0
import QtGraphicalEffects 1.0
import QtQuick.Layouts 1.1
import Ubuntu.Components 1.3
import Ubuntu.Components.ListItems 1.3
import Ubuntu.Components.Popups 1.3
import Ubuntu.Components.ListItems 1.3 as ListItem
import Qt.labs.folderlistmodel 2.1

Page {
    property alias pass: password
    property alias cmbKeySelector: combo
    property alias actTheme: themeAction

    header: PageHeader {
      id: pageHeader
      title: i18n.tr(appTitle)
      trailingActionBar {
        actions: [
          Action {
            iconName: "help"
            text: i18n.tr("About")
            onTriggered: PopupUtils.open(about)
          },
          Action {
            id: themeAction
            iconName: ""
            text: i18n.tr("Swap")            
            onTriggered: {                
                switchTheme()
                themeAction.iconName = getTheme()
                saveTheme()
            }                      
          },

        Action {
            text: i18n.tr("Settings")
            iconName: "settings"
            onTriggered: {
                pageStack.push(settingsForm)
                settingsForm.loadSettings()
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
               iconName: "edit-undo"
               text: i18n.tr("Close")
               onTriggered: {
                   pageStack.clear()
                   databaseListView.setDatabaseMode()
                   password.text = ''
                   keyFileName = ''
                   pageStack.push(databaseListView)
               }
           }]
      }
    }

    Column {
        spacing: units.gu(2)
        anchors {
            margins: units.gu(2)
            fill: parent
            topMargin: pageHeader.height + units.gu(2)
        }

        Label {
            id: dbText
            width: parent.width
            text: databaseFileName
        }

        PasswordInput {
            id: password
            inputWidth: parent.width
        }

        KeySelector {
            id: combo
            width: parent.width
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
