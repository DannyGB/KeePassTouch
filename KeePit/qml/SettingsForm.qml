/*
* This file is part of KeePit
*
* Copyright (C) 2019 Dan Beavon
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
import QtQuick.Layouts 1.1
import Ubuntu.Components.ListItems 1.3
import Ubuntu.Components 1.3

Page {        

    property var loadedSettings: {}

    function loadSettings() {
        loadedSettings = settings.getSettings() 
        var themeName = loadedSettings["theme"]
        cboTheme.text = (themeName === settings.dark)
            ? i18n.tr("Dark")
            : i18n.tr("Light")
        txtTimeout.text = loadedSettings["timeout"] 
        swTimeout.checked = loadedSettings["timeoutEnabled"] != "0"
    }

    header: PageHeader {
      id: pageHeader
      title: i18n.tr("Settings")      
    }

    Column {
        spacing: units.gu(2)
        anchors {
            margins: units.gu(2)
            fill: parent
            topMargin: pageHeader.height + units.gu(2)
        }

        Label {
            width: parent.width
            text: i18n.tr("Theme")
        }

        ComboButton {
            id: cboTheme
            text: "Theme"
            onClicked: expanded = false
            UbuntuListView {
                width: parent.width
                height: cboTheme.comboListHeight
                model: ListModel {                    
                    ListElement {
                        name: "Dark"
                    }
                    ListElement {
                        name: "Light"
                    }
                }
                delegate: Standard {
                    text: name
                    onClicked: {
                        cboTheme.text = text;
                        cboTheme.expanded = false;
                    }
                }
            }            
        }       

        Label {
            width: parent.width
            text: i18n.tr("Timeout Enabled")
        }

        Switch {
            id: swTimeout            
        }

        Label {
            width: parent.width
            text: i18n.tr("Timeout")
        }

        TextField {
            id: txtTimeout
            width: parent.width
        }

        Button {
            objectName: "button"
            width: parent.width
            color: UbuntuColors.green
            text: i18n.tr("Save")

            onClicked: {
                loadedSettings.theme = (cboTheme.text == "Dark")
                    ? "Ubuntu.Components.Themes.SuruDark"
                    : "Ubuntu.Components.Themes.Ambiance"
                loadedSettings.timeout = txtTimeout.text
                loadedSettings.timeoutEnabled = swTimeout.checked
                settings.saveSettings(loadedSettings)
                pageStack.pop()
            }
        }
    }
}