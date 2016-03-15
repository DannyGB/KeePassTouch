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
                iconName: "edit-undo"
                text: i18n.tr("Close")
                onTriggered: {
                    pageStack.clear()
                    databaseListView.setDatabaseMode()
                    password.text = ''
                    keyFileName = ''
                    pageStack.push(databaseListView)
                }
              },
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
            width: parent.width
            text: databaseFileName
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
            width: parent.width            
            text: keyFileName
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
                database.openFile(databaseFilePath, password.text, keyFilePath);
            }
        }
    }
}
