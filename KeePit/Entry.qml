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
import Ubuntu.Components 1.3
import Ubuntu.Components.Popups 1.3
import KeePass3 1.0

Page {

    property alias txtEntityPass: password
    property alias btnRevealPass: reveal
    title: i18n.tr(database.selectedEntry.title)
    head {
            actions: [
                Action {
                    text: i18n.tr("Copy")
                    iconName: "edit-copy"
                    onTriggered: Clipboard.push(password.text);
                },
                /*
                Action {
                    text: i18n.tr("Settings")
                    iconName: "settings"
                    onTriggered: PopupUtils.open(settingsDisabledComponent)
                }*/
                Action {
                    iconName: "system-log-out"
                    text: i18n.tr("Logout")
                    onTriggered: {
                        reset();
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
             text:  database.selectedEntry.uuid
             visible: false;
            }

            Label {
             text: i18n.tr("Username")
             width: parent.width
             color: UbuntuColors.darkAubergine
            }

            TextField {
                id: userName
                text: database.selectedEntry.username
                readOnly: true
                color: UbuntuColors.darkAubergine
            }

            Label {
             text: i18n.tr("Password")
             width: parent.width
             color: UbuntuColors.darkAubergine
            }

            TextField {
                id: password
                text: database.selectedEntry.password
                echoMode : TextInput.Password
                readOnly: true
                width: parent.width
                color: UbuntuColors.darkAubergine
            }

            Button {
                id: reveal
                text: i18n.tr("Show")
                width: parent.width
                font.pointSize: 25
                color: UbuntuColors.green
                onClicked: {
                    if(password.echoMode == TextInput.Normal) {
                        password.echoMode = TextInput.Password;
                        reveal.text = i18n.tr("Show")
                    } else {
                        password.echoMode = TextInput.Normal;
                        reveal.text = i18n.tr("Hide")
                    }
                }
            }

            Label {
             text: i18n.tr("Url")
             width: parent.width
             color: UbuntuColors.darkAubergine
            }

            TextField {
                id: url
                text: database.selectedEntry.url
                readOnly: true
                width: parent.width
                color: UbuntuColors.darkAubergine
            }

            Label {
             text: i18n.tr("Notes")
             width: parent.width
             color: UbuntuColors.darkAubergine
            }

            TextArea {
                id: notes
                text: database.selectedEntry.notes
                readOnly: true
                width: parent.width
                color: UbuntuColors.darkAubergine
            }
    }
}
