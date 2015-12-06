import QtQuick 2.0
import Ubuntu.Components 1.2
import Ubuntu.Components.Popups 1.2
import KeePass3 1.0

Page {

    function onSelected(index) {
        pageStack.push(entry);
        listView.currentIndex = index;
    }

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

        Rectangle {
            width: parent.width
            height: parent.height
            Component {
                id: entryDelegate
                Item {
                    width: listView.width
                    height: 80
                    Column {
                        Text { text: title }
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: onSelected(index)
                    }
                }
            }

        ListView {
            id: listView
            anchors {
                margins: units.gu(2)
                fill: parent
            }
            spacing: 5
            //model: EntryModel {}
            model: passwordEntryModel
            //model: listModel
            /*model: ListModel {
                ListElement {
                    title: pe.title
                }
            }*/

            delegate: entryDelegate
            focus: true
            }
        }
    }

   /* PasswordEntry {
        id: pe
        title: 'sample'
    }*/
}
