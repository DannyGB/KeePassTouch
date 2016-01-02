import QtQuick 2.0
import QtQuick.LocalStorage 2.0
import QtGraphicalEffects 1.0
import Ubuntu.Components 1.2
import Ubuntu.Components.ListItems 1.0
import Ubuntu.Components.Popups 1.0
//import KeePass 1.0

Page {
    property alias pass: password
    title: i18n.tr(appTitle)
    head {
        actions: [
            Action {
              iconName: "help"
              text: i18n.tr("About")
              onTriggered: PopupUtils.open(about)
            }]
    }

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
            text: databaseFilePath
            readOnly: true
        }

        TextField {
            id: password
            placeholderText: "enter your password"
            width: parent.width
            echoMode : TextInput.PasswordEchoOnEdit
        }

        TextField {
            id: key
            placeholderText: "type key url here"
            width: parent.width            
            text: keyFilePath
            readOnly: true
        }

        Button {
            objectName: "button"
            width: parent.width
            color: UbuntuColors.green
            text: i18n.tr("Select Key")

            onClicked: {
                databaseListView.setKeyMode();
                pageStack.push(databaseListView)
            }
        }

        Button {
            objectName: "button"
            width: parent.width
            color: UbuntuColors.orange
            text: i18n.tr("Open Database")

            onClicked: {
                database.openFile(i18n.tr(dbText.text), i18n.tr(password.text), i18n.tr(key.text));                
            }
        }
    }
}
