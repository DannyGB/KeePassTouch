import QtQuick 2.0
import Ubuntu.Components 1.2
import Ubuntu.Components.Popups 1.2
import KeePass3 1.0

Page {
    // Could be the name of the group
    title: i18n.tr("KeePass")
    head {
            actions: [
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

        Flow {

            anchors.fill: parent
            spacing: 5

            Label {
             text: filesystem.selectedEntry.uuid
             visible: false;
            }

            Label {
             text: "Title"
            }

            TextField {
                id: title
                text: filesystem.selectedEntry.title
                width: parent.width
                readOnly: true
            }

            Label {
             text: "Password"
             width: parent.width
            }

            TextField {
                id: password
                text: filesystem.selectedEntry.password
                //width: parent.width / 2
                //echoMode : TextInput.PasswordEchoOnEdit
                readOnly: true
            }

            Switch {
                id: passwordProtected
                checked: filesystem.selectedEntry.passwordProtected
                visible: false
            }

            Button {
                text: "..."
                //width: parent.width / 2
                onClicked: {
                    var pwrd
                    if(passwordProtected.checked) {
                        pwrd = fileSystem.decryptPassword(password.text)
                    } else {
                        pwrd = password.text
                    }

                    password.text = pwrd
                    password.echoMode = TextInput.Normal
                }

            }

            Label {
             text: "Username"
             width: parent.width
            }

            TextField {
                id: userName
                text: filesystem.selectedEntry.username
                readOnly: true
            }
        }
    }      
}
