/*
* This file is part of KeePit
*
* Copyright (C) 2016 Dan Beavon
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

import QtQuick 2.4
import Ubuntu.Components 1.3
import Ubuntu.Components.Popups 1.3
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
    property var selectedDatabaseToDelete: ({})
    property var pageTitleStack: []
    property var timeoutSettings: {
        "enabled": false,
        "interval": 0
    }

    function goHome() {
        database.loadHome()
        previousDepth = null
        pageTitleStack.length = 0
        listEntryItems.searching = false
        pageStack.clear()
        pageStack.push(listEntryItems)
    }

    Component.onCompleted: {        
        setTheme()             
    }    

   Database {
        id: database
        property var selectedEntry
        onError: {
            PopupUtils.open(dialog, '', {text: i18n.tr(msg)})
        }
        onSuccess: {
            pageStack.clear()
            pageStack.push(listEntryItems)
            setTimer()     
        }
    }

    PageStack {
           id: pageStack
           Component.onCompleted: { 
                var lastOpenDb = {
                    name: settings.getSetting("lastOpenDbName"),
                    path: settings.getSetting("lastOpenDbPath")
                }

                if(lastOpenDb.name === "" || lastOpenDb.path === "" ) {              
                    push(databaseListView)
                } else {
                    databaseFileName = lastOpenDb.name
                    databaseFilePath = lastOpenDb.path
                    push(openDatabase)
                }
           }
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
                            pageTitleStack.pop()
                            if(previousEntry.searching) {
                                goHome()
                            }
                       }
                       else if(previousDepth < depth) { // Forwards
                           database.selectBranch(previousEntry.UUID);
                           pageTitleStack.push(previousEntry.title)
                       }
                    }
                    
                    console.debug("Main: " + pageTitleStack[pageTitleStack.length-1])
                    listEntryItems.pageHeaderTitle = pageTitleStack.length > 0 
                        ? pageTitleStack[pageTitleStack.length-1]
                        : i18n.tr(appTitle)
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
               onDatabaseSelected: {
                    settings.saveSetting("lastOpenDbName", dbName)
                    settings.saveSetting("lastOpenDbPath", dbPath)
               }
           }

           Importer {
               id: importer
           }

           CreateDatabase {
               id: createDatabase
           }

           Settings {
               id: settings
               onSettingsChanged: {                   
                   setTimer()
                   setTheme()
               }
           }

           SettingsForm {
               id: settingsForm
               visible: false
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
             title: i18n.tr("About (KeePit v3.1)")
             text: i18n.tr("Copyright &copy; "+ currentDate.toLocaleString(locale, "yyyy") +" Dan Beavon<br/>This software is distributed under the terms of the GNU General Public License v3")
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

    function setTimer() {
        getTimeout()
        startTimer()
    }

    function startTimer() {
        console.debug("startTimer: " + timeoutSettings.enabled + "("+timeoutSettings.interval+")")
        if(timeoutSettings.enabled) {
            resetTimer.interval = timeoutSettings.interval
            resetTimer.stop()
            resetTimer.start()
        } else {
            resetTimer.stop()
        }
    }

    function resetLogoutTimer() {
        if(timeoutSettings.enabled) {
            resetTimer.restart()
        } else {
            resetTimer.stop()
        }
    }

    function setTheme() {
        readTheme()
        openDatabase.actTheme.iconName = getTheme()
        listEntryItems.actTheme.iconName = getTheme()        
    }

    function getTheme() {
        return (theme.name == settings.dark) 
            ? "torch-off" 
            : "torch-on"
    }

    function switchTheme() {
        theme.name = (theme.name == settings.dark)
            ? settings.light
            : settings.dark
    }    

    function saveTheme() {
        settings.saveSetting("theme", theme.name);
    }

    function readTheme() {
        theme.name = settings.getSetting("theme")
    }

    function getTimeout() {
        timeoutSettings.enabled = settings.getSetting("timeoutEnabled") != "0"
        timeoutSettings.interval = settings.getSetting("timeout")
    }
}
