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
import Ubuntu.Components 1.2
import Ubuntu.Components.Popups 1.2
import KeePass3 1.0
import QtQml 2.2

/*!
    \brief MainView with a Label and Button elements.
*/

MainView {    
    // objectName for functional testing purposes (autopilot-qt5)
    objectName: "mainView"

    // Note! applicationName needs to match the "name" field of the click manifest
    applicationName: "keepit.dannygb"

    /*
     This property enables the application to change orientation
     when the device is rotated. The default is false.
    */
    automaticOrientation: true
    //backgroundColor: "#000"
    //headerColor: "#000"

    property string appLocation: '/home/phablet/.local/share/keepit.dannygb/Documents'
    property string appTitle: 'KeePit'
    width: units.gu(100)
    height: units.gu(75)
    property var previousEntry: ({})
    property var previousDepth
    property string databaseFilePath
    property string keyFilePath
    property string databaseFileName
    property string keyFileName
    property date currentDate: new Date()
    property var locale: Qt.locale()

    property var selectedDatabaseToDelete: ({});

   Database {
        id: database
        property var selectedEntry
        onError: {
            PopupUtils.open(dialog, '', {text: i18n.tr(msg)});
        }
        onSuccess: {            
            pageStack.clear();
            pageStack.push(listEntryItems);
            resetTimer.start()
        }        
    }

    PageStack {
           id: pageStack
           Component.onCompleted: push(databaseListView)
           onCurrentPageChanged: {
               if(currentPage != null) {
                currentPage.forceActiveFocus()
               }
           }

           onDepthChanged: {               
               // This logic keeps track of where we are in the tree so we can move backwards and forwards
               // in the PageStack keeping a consistent position
               // See also listEntryItems(.qml).onSelected to see where we set the previousEntry details
               // whenever the user chooses an entry from the ListView
               if(previousDepth) {
                   if(previousEntry.UUID !== undefined) {
                       if(previousDepth > depth) { // Backwards
                            previousEntry.UUID = database.reloadBranch(previousEntry.UUID, previousEntry.entryType)
                       }
                       else if(previousDepth < depth) { // Forwards
                           database.selectBranch(previousEntry.UUID);
                       }
                   }                   
               }

               resetLogoutTimer()
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

           Selector {
               id: databaseListView
               visible: false
           }

           Importer {
               id: importer
           }
    }

    Component {
            id: settingsDisabledComponent

            Dialog {
                id: settingsDisabledDialog
                title: i18n.tr("Settings")

                Button {
                    color: UbuntuColors.orange
                    text: i18n.tr("Close Database")
                    onClicked: {
                        PopupUtils.close(settingsDisabledDialog)
                        reset()
                    }
                }

                Button {
                    text: i18n.tr("Cancel")
                    onClicked: PopupUtils.close(settingsDisabledDialog)
                }
            }
        }

    Component {
         id: dialog
         Dialog {
             id: dialogue
             title: i18n.tr("Error")
             text: i18n.tr("An error has occurred")
             Button {
                 text: i18n.tr("Ok")
                 onClicked: PopupUtils.close(dialogue)
             }
         }
    }

    Component {
        id: about
         Dialog {
             id: aboutDialog
             title: i18n.tr("About (KeePit v2.1)")
             text: i18n.tr("Copyright &copy; "+ currentDate.toLocaleString(locale, "yyyy") +" Dan Beavon<br/>This software is distributed under the terms of the GNU General Public License v2 or Later")
             Button {
                 text: i18n.tr("Ok")
                 onClicked: PopupUtils.close(aboutDialog)
                 color: UbuntuColors.green
             }
         }
    }

    Component {
         id: infoPopup
         Dialog {
             id: infoPopupDialogue
             title: i18n.tr("Info")
             text: i18n.tr("Information: ")
             Button {
                 text: i18n.tr("Ok")
                 onClicked: PopupUtils.close(infoPopupDialogue)
                 color: UbuntuColors.green
             }
         }
    }

    Component {
         id: searchPopup
         Dialog {
             id: searchPopupDialogue
             title: i18n.tr("Search")
             TextField {
                 id: searchTerm
                 placeholderText: i18n.tr("search term")
                 width: parent.width
                 color: UbuntuColors.darkAubergine
             }
             Button {
                 text: i18n.tr("Search")
                 onClicked: {
                     if(searchTerm.text !== '') {
                        PopupUtils.close(searchPopupDialogue)
                        database.search(searchTerm.text)
                     }
                 }
                 color: UbuntuColors.green
             }
             Button {
                 text: i18n.tr("Cancel")
                 onClicked: PopupUtils.close(searchPopupDialogue)
                 color: UbuntuColors.lightGrey
             }
         }
    }

    Component {
         id: deletePopup
         Dialog {
             id: deletePopupDialogue
             title: i18n.tr("Delete")

             Label {
                 text: selectedDatabaseToDelete.name
             }

             Button {
                 text: i18n.tr("Delete")
                 onClicked: {
                     database.deleteFile(selectedDatabaseToDelete.path)
                     PopupUtils.close(deletePopupDialogue)
                 }
                 color: UbuntuColors.red
             }
             Button {
                 text: i18n.tr("Cancel")
                 onClicked: PopupUtils.close(deletePopupDialogue)
             }
         }
    }

    Timer {
        id: resetTimer
        interval: 60000
        running: false
        repeat: false
        onTriggered: reset()
    }

    function resetOpenDatabasePage() {
        openDatabase.pass.text = '';
        openDatabase.cmbKeySelector.text = i18n.tr("Select Key");
    }

    function reset() {
        database.closeFile();
        pageStack.clear();
        databaseListView.setDatabaseMode();
        pageStack.push(databaseListView);
        resetOpenDatabasePage();
        resetTimer.stop();
        //Clipboard.clear() // Consistently crashes the app on phone (not desktop though) see bug: https://bugs.launchpad.net/ubuntu/+source/ubuntu-ui-toolkit/+bug/1457361
    }

    function resetLogoutTimer() {
        resetTimer.restart();
    }
}
