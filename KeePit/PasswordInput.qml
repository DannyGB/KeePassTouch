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
import Ubuntu.Components.ListItems 1.3
import Ubuntu.Components.Popups 1.3
import Ubuntu.Components.ListItems 1.3 as ListItem
import Qt.labs.folderlistmodel 2.1

    Column {

        property alias toggleText: toggle.text
        property alias toggleEnabled: toggle.enabled
        property alias placeholderText: password.placeholderText
        property alias inputWidth: password.width
        property alias text: password.text
        property alias toggleVisible: toggle.visible
        property alias color: password.color
        property alias readOnly: password.readOnly

        signal toggleClicked(bool state)

        TextField {
            id: password
            placeholderText: "Enter your password"
            echoMode : TextInput.Password
            readOnly: false
        }

        ListItem.Standard {
            id: toggle
            visible: true
            text: i18n.tr("Show password")
            enabled: true
            control: Switch {
                id: showPassword
                onClicked: {
                    password.echoMode = (password.echoMode == TextInput.Normal ? TextInput.Password : TextInput.Normal)
                }
                onCheckedChanged: {
                    toggleClicked(checked)
                }
           }
        }
    }
