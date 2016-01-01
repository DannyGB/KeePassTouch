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
    //backgroundColor: "#000"
    //headerColor: "#000"

    property string appTitle: 'KeePT'
    width: units.gu(100)
    height: units.gu(75)
    property var previousEntry: ({})
    property var previousDepth
   Database {
        id: database
        property var selectedEntry
        onError: {
            PopupUtils.open(dialog, '', {text: i18n.tr(msg)});
        }
        onSuccess: {            
            pageStack.clear();
            pageStack.push(listEntryItems);
        }        
    }

    PageStack {
           id: pageStack
           Component.onCompleted: push(openDatabase)
           onCurrentPageChanged: {
               currentPage.forceActiveFocus()
               openDatabase.pass.text = ''
               entry.pass.echoMode = TextInput.Password
           }

           onDepthChanged: {
               // This logic keeps track of where we are in the tree so we can move backwards and forwards
               // in the PageStack keeping a consistent position
               // See also listEntryItems(.qml).onSelected to see where we set the previousEntry details
               // whenever the user chooses an entry from the ListView
               if(previousDepth) {
                   if(previousDepth > depth) { // Backwards
                        previousEntry.UUID = database.reloadBranch(previousEntry.UUID, previousEntry.entryType)
                   }
                   else if(previousDepth < depth) { // Forwards
                       database.selectBranch(previousEntry.UUID);
                   }
               }

               previousDepth = depth
           }

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
                    color: UbuntuColors.orange
                    text: i18n.tr("Sign out")
                    onClicked: {
                        PopupUtils.close(settingsDisabledDialog)
                        filesystem.closeFile()
                        pageStack.clear()
                        pageStack.push(openDatabase)
                    }
                }

                Button {
                    text: "Cancel"
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

    Component {
        id: about
         Dialog {
             id: aboutDialog
             title: "About"
             text: "Copyright &copy; 2015 Dan Beavon<br/>This software is distributed under the terms of the GNU General Public License v2 or Later"
             Button {
                 text: "Ok"
                 onClicked: PopupUtils.close(aboutDialog)
                 color: UbuntuColors.green
             }
         }
    }

    Component {
         id: infoPopup
         Dialog {
             id: infoPopupDialogue
             title: "Info"
             text: "Information: "
             Button {
                 text: "Ok"
                 onClicked: PopupUtils.close(infoPopupDialogue)
                 color: UbuntuColors.green
             }
         }
    }
}
