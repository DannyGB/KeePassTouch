import QtQuick 2.0
import QtQuick.LocalStorage 2.0
import QtGraphicalEffects 1.0
import Ubuntu.Components 1.2
import Ubuntu.Components.ListItems 1.0
import Ubuntu.Components.Popups 1.0
//import KeePass 1.0

Page {
    title: i18n.tr(appTitle)
    Column {
        spacing: units.gu(1)
        anchors {
            margins: units.gu(2)
            fill: parent
        }

        TextField {
            id: dbText
            placeholderText: "type database url here"
            width: parent.width
            //text: "/home/phablet/Documents/testwithkeyfile.kdbx"
            text: "../../testwithkeyfile.kdbx"
        }

        TextField {
            id: password
            placeholderText: "enter your password"
            width: parent.width
            echoMode : TextInput.PasswordEchoOnEdit
            text: "1234"
        }

        TextField {
            id: key
            placeholderText: "type key url here"
            width: parent.width
            //text: "/home/phablet/Documents/testwithkeyfile.key"
            text: "../../testwithkeyfile.key"
        }

        Button {
            objectName: "button"
            width: parent.width
            color: UbuntuColors.green
            text: i18n.tr("Open Database")

            onClicked: {
                filesystem.openFile(i18n.tr(dbText.text), i18n.tr(password.text), i18n.tr(key.text));
            }
        }
    }
}
