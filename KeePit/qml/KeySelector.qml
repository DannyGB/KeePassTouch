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

import QtQuick 2.0
import Ubuntu.Components 1.3
import Ubuntu.Components.ListItems 1.3
import Ubuntu.Components.ListItems 1.3 as ListItemImport
import Qt.labs.folderlistmodel 2.1
import QtQuick.Layouts 1.1

Column {

    id: col
    signal toggleChecked(bool state)
    signal keySelected(string fileName, string filePath)
    property alias nameFilters: folderModel.nameFilters
    property alias showDirs: folderModel.showDirs
    property alias folder: folderModel.folder
    property alias listHeight: list.height
    property alias text: key_selector.text
    property alias labelText: toggle.text

    ListItemImport.Standard {
        id: toggle
        visible: true
        text: i18n.tr("Use Key")
        enabled: true
        control: Switch {
          id: useKey
          checked: false
          onClicked: {
              importer.visible = checked
          }
          onCheckedChanged: {
              toggleChecked(checked)
          }
        }
    }

    ListItem {
        id: "importer"
        visible: false
        height: 500

        ComboButton {
          id: key_selector
          width: parent.width
          text: i18n.tr("Select Key")
          onClicked: {
              expanded = false
          }

          ListView {
              id: list
              height: 200

              FolderListModel {
                  id: folderModel
                  nameFilters: ["*.*"]
                  showDirs: false
                  folder: 'file:'+ appLocation
              }

              model: folderModel
              delegate: Standard {
                  text: fileName
                  onClicked: {
                        key_selector.text = text;
                        key_selector.expanded = false;
                        col.keySelected(fileName, filePath)
                    }
              }
          }
      }
    }
}
