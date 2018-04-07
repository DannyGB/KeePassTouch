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

import QtQuick 2.4
import QtQuick.Layouts 1.1
import Ubuntu.Components 1.3 /* 1.2 */
import Ubuntu.Components.Popups 1.3 /* 1.2 */
import KeePass3 1.0

Page {    

    /**
      * Handles the MouseArea.onClicked event of the entryDelegate
      */
    function onSelected(index, model) {
        // A variable property on Database (Main.qml)
        database.selectedEntry = model

        // A variable property on MainView (Main.qml)
        previousEntry = {
            entryType : database.selectedEntry.entryType,
            UUID : database.selectedEntry.uuid
        }

        // See PageStack.onDepthChanged for how the pageStack and entry model are kept in synch
        if(database.selectedEntry.entryType === 2) { // is a password entry so push password page
            entry.txtEntityPass.echoMode = TextInput.Password
            pageStack.push(entry);
        } else if(database.selectedEntry.entryType === 1) { // is a further branch push another level
            pageStack.push(listEntryItems);
        }

        listView.currentIndex = index;
    }

    function getIconName(model) {
        if(model.entryType === 2) {
            return "note";
        } else {
            return "document-open";
        }
    }

    // Could be the name of the group
    title: i18n.tr(appTitle)
    head {
            actions: [
                Action {
                  iconName: "home"
                  text: i18n.tr("Home")
                  onTriggered: {
                      database.loadHome()
                      previousDepth = null
                      pageStack.clear()
                      pageStack.push(listEntryItems);
                  }
                },
                Action {
                  iconName: "search"
                  text: i18n.tr("Search")
                  onTriggered: PopupUtils.open(searchPopup)
                },
                Action {
                    text: i18n.tr("Settings")
                    iconName: "settings"
                    onTriggered: PopupUtils.open(settingsDisabledComponent)                    
                }
            ]
    }

    Column {
        spacing: units.gu(1)
        anchors {
            margins: units.gu(2)
            fill: parent
        }

            Component {
                id: entryDelegate
                Item {
                    width: listView.width
                    height: units.gu(5)
                    GridLayout {
                        columns: 2
                        Icon {
                            width: 40
                            height: 40
                            name: getIconName(model)
                            color: UbuntuColors.darkAubergine
                        }
                        Text {
                            text: title
                            font.pointSize: 35  /* Size 12 */
                            color: UbuntuColors.darkAubergine
                        }
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: onSelected(index, model)
                    }                    
                }                
            }

            UbuntuListView {
                id: listView
                height: parent.height
                width: parent.width
                spacing: 5
                model: passwordEntryModel //passwordEntryModel is the initial model from the C++ side of the application (See main.cpp)
                delegate: entryDelegate
                focus: true
              }
        }
}
