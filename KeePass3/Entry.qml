import QtQuick 2.0
import Ubuntu.Components 1.2
import Ubuntu.Components.Popups 1.2
import KeePass3 1.0

Page {
    title: i18n.tr(filesystem.selectedEntry.title)
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

            Label {
             text: filesystem.selectedEntry.uuid
             visible: false;
            }           

            Label {
             text: "Username"
             width: parent.width
             color: UbuntuColors.darkAubergine
            }

            TextField {
                id: userName
                text: filesystem.selectedEntry.username
                readOnly: true
                color: UbuntuColors.darkAubergine
            }

            Label {
             text: "Password"
             width: parent.width
             color: UbuntuColors.darkAubergine
            }

            TextField {
                id: password
                text: filesystem.selectedEntry.password                
                echoMode : TextInput.Password
                readOnly: true
                width: parent.width
                color: UbuntuColors.darkAubergine
            }

            Button {
                text: "..."
                width: parent.width
                font.pointSize: units.gu(2)
                color: UbuntuColors.green
                onClicked: {                    
                    if(password.echoMode == TextInput.Normal) {
                        password.echoMode = TextInput.Password;
                    } else {
                        password.echoMode = TextInput.Normal;
                    }
                }
            }           
    }      
}
