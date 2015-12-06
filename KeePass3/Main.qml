import QtQuick 2.0
import Ubuntu.Components 1.2
import Ubuntu.Components.Popups 1.2
import KeePass3 1.0
/*!
    \brief MainView with a Label and Button elements.
*/

MainView {
    // objectName for functional testing purposes (autopilot-qt5)
    objectName: "mainView"

    // Note! applicationName needs to match the "name" field of the click manifest
    applicationName: "keepass3.dannygb"

    /*
     This property enables the application to change orientation
     when the device is rotated. The default is false.
    */
    automaticOrientation: true

    width: units.gu(100)
    height: units.gu(75)

   Filesystem {
        id: filesystem
        onError: {
            PopupUtils.open(dialog, '', {text: i18n.tr(msg)});
        }
        onSuccess: {            
            pageStack.push(listEntryItems);
        }
    }

    PageStack {
           id: pageStack
           Component.onCompleted: push(openDatabase)
           onCurrentPageChanged: currentPage.forceActiveFocus()

           OpenDatabase {
               id: openDatabase
               visible: false
           }

          ListEntryItems {
               id: listEntryItems
               visible: false
           }

           Entry {
               id: entry
               visible: false
           }
    }

    Component {
            id: settingsDisabledComponent

            Dialog {
                id: settingsDisabledDialog
                title: i18n.tr("Settings")

                Button {
                    color: 'orange'
                    text: i18n.tr("Sign out")
                    onClicked: {
                        PopupUtils.close(settingsDisabledDialog)
                        filesystem.closeFile()
                        pageStack.clear()
                        pageStack.push(openDatabase)

                    }
                }

                Button {
                    text: "Close"
                    onClicked: PopupUtils.close(settingsDisabledDialog)
                }
            }
        }

    Component {
         id: dialog
         Dialog {
             id: dialogue
             title: "Error"
             text: "An error has occurred"
             Button {
                 text: "Ok"
                 onClicked: PopupUtils.close(dialogue)
             }
         }
    }

}
