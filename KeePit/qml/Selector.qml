/*
* This file is part of KeePit
*
*  Copyright (C) 2018 Dan Beavon
*  Copyright (C) 2016 Simon Stuerz <stuerz.simon@gmail.com>               *
*  Copyright (C) 2016 Michael Zanetti  (KodiMote)
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
import QtQuick.Layouts 1.1
import Ubuntu.Components 1.3
import Ubuntu.Components.Popups 1.3
import Ubuntu.Components.ListItems 1.3
import Ubuntu.Components.Pickers 1.3
import Ubuntu.Content 1.1
import Qt.labs.folderlistmodel 2.1

Page {

    property string pageTitle : appTitle + " - " + i18n.tr("Databases")
    property bool keyMode: false

    function setKeyMode() {
        keyMode = true
        pageTitle = i18n.tr("Keys")
        folderModel.nameFilters = ["*.*"]
        importer.headerText = i18n.tr('Import key from')
        //actCreateDb.visible = false;
    }

    function setDatabaseMode() {
        keyMode = false
        pageTitle = appTitle + " - " + i18n.tr("Databases")
        folderModel.nameFilters = ["*.kdbx"]
        importer.headerText = i18n.tr('Import database from')
        //actCreateDb.visible = true;
    }

    function onDatabaseSelected(index, model) {
        if(keyMode) {
            keyFilePath = model.filePath
            keyFileName = model.fileName
        } else {
            databaseFilePath = model.filePath
            databaseFileName = model.fileName
            keyFilePath = ''
            keyFileName = ''
        }

        pageStack.clear()
        pageStack.push(openDatabase)
        sourcesView.currentIndex = index;
    }

    title: i18n.tr(pageTitle)

    Column {

        spacing: units.gu(1)
        anchors {
            margins: units.gu(2)
            fill: parent
        }

        FolderListModel {
            id: folderModel
            nameFilters: ["*.kdbx"]
            showDirs: false
            // I don't know yet how to make this get the correct folder for my app!
            folder: 'file:'+ appLocation
            sortField: 'Name'
        }

        UbuntuListView {
            id: sourcesView
            focus: true
            height: parent.height
            width: parent.width
            spacing: 5
            model: folderModel
            delegate: ListItem {
                width: sourcesView.width
                height: units.gu(5)
                Text {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.leftMargin: units.gu(2)
                    anchors.rightMargin: units.gu(2)
                    text: fileName
                    color: theme.palette.normal.baseText
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        onDatabaseSelected(index, model)
                    }
                }
                leadingActions: ListItemActions {
                    actions: [
                        Action {
                            iconName: "delete"
                            onTriggered: {
                                selectedDatabaseToDelete.path = folderModel.get(index, "filePath")
                                selectedDatabaseToDelete.name = folderModel.get(index, "fileName")
                                PopupUtils.open(deletePopup)
                            }
                        }
                    ]
                }
            }
        }
    }

    Item {
        id: root
        anchors { left: parent.left; right: parent.right; bottom: parent.bottom }
        height: units.gu(2)
        z: 3

        property real progress: 0

        Rectangle {
            anchors {fill: parent; topMargin: -units.gu(200) }
            color: "#88000000"
            opacity: root.progress
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    root.progress = 0;
                    mouse.accepted = true;
                }
                enabled: root.progress > 0
            }
        }

        MouseArea {
            anchors { left: parent.left; right: parent.right; bottom: parent.bottom }
            height: contentRect.height
            property bool ignoring: false

            property var gesturePoints: new Array()

            onPressed: {
                gesturePoints = new Array();
                ignoring = false;
                if (root.progress == 0 && mouseY < height - units.gu(2)) {
                    mouse.accepted = false;
                    ignoring = true;
                }
            }

            onMouseYChanged: {
                if (ignoring) {
                    return;
                }
                root.progress = Math.min(1, (height - mouseY) / height)
                gesturePoints.push(mouseY)
            }

            onReleased: {
                var oneWayFlick = true;
                var upwards = gesturePoints[1] < gesturePoints[0];
                for (var i = 1; i < gesturePoints.length; i++) {
                    if (upwards && gesturePoints[i] > gesturePoints[i-1]) {
                        oneWayFlick = false;
                        break;
                    } else if(!upwards && gesturePoints[i] < gesturePoints[i-1]) {
                        oneWayFlick = false;
                        break;
                    }
                }

                if (oneWayFlick && upwards) {
                    root.progress = 1;
                } else if (oneWayFlick && !upwards) {
                    root.progress = 0;
                } else if (root.progress > .5) {
                    root.progress = 1;
                } else {
                    root.progress = 0;
                }
            }

            Rectangle {
                id: contentRect

                anchors {
                    left: parent.left
                    right: parent.right
                    top: parent.bottom
                    topMargin: -units.gu(2) - root.progress * (height - units.gu(2))
                }

                height: contentColumn.height + units.gu(4)

                Behavior on anchors.topMargin {
                    UbuntuNumberAnimation {}
                }

                Rectangle {
                    id: borderRectangle
                    anchors { left: contentRect.left; top: contentRect.top; right: contentRect.right }
                    height: units.gu(2)

                    UbuntuShape {
                        anchors.centerIn: parent
                        height: units.gu(1)
                        width: units.gu(5)
                        radius: "medium"
                        color: UbuntuColors.inkstone
                    }
                }

                RowLayout {
                    id: contentColumn
                    anchors {
                        left: parent.left
                        right: parent.right
                        top: parent.top
                        margins: units.gu(2)
                    }

                      Button {
                        Layout.fillWidth: true
                        iconName: "import"
                        text: i18n.tr("Import")
                        onTriggered: {
                          pageStack.push(importer.pickerPage)
                        }
                      }

                      Button {
                        visible: false
                        Layout.fillWidth: true
                        id: actCreateDb
                        iconName: "add"
                        text: i18n.tr("Create")
                        onTriggered: {
                          pageStack.push(createDatabase.createDatabasePage)
                        }
                      }
              }
          }
      }
    }
}
