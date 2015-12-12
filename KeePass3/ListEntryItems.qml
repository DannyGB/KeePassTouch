import QtQuick 2.0
import Ubuntu.Components 1.2
import Ubuntu.Components.Popups 1.2
import KeePass3 1.0

Page {    

    /**
      * Handles the MouseArea.onClicked event of the entryDelegate
      */
    function onSelected(index, model) {
        // A variable property on FileSystem (Main.qml)
        filesystem.selectedEntry = model

        // A variable property on MainView (Main.qml)
        previousEntry = {
            entryType : filesystem.selectedEntry.entryType,
            UUID : filesystem.selectedEntry.uuid
        }

        // See PageStack.onDepthChanged for how the pageStack and entry model are kept in synch
        if(filesystem.selectedEntry.entryType === 2) { // is a password entry so push password page
            pageStack.push(entry);
        } else if(filesystem.selectedEntry.entryType === 1) { // is a further branch push another level
            pageStack.push(listEntryItems);
        }

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
                        onClicked: onSelected(index, model)
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
            model: passwordEntryModel //passwordEntryModel is the initial model from the C++ side of the application (See main.cpp)
            delegate: entryDelegate
            focus: true
            }
        }
    }
}
