import QtQuick 2.0
import Ubuntu.Components 1.2
import Ubuntu.Components.Popups 1.2
import Ubuntu.Components.ListItems 1.0
import Ubuntu.Content 1.1
import Qt.labs.folderlistmodel 2.1

Page {

    property string pageTitle : i18n.tr("Available Databases")
    property bool keyMode: false

    function setKeyMode() {
        keyMode = true
        pageTitle = i18n.tr("Available Keys")
        folderModel.nameFilters = ["*.key"]
    }

    function setDatabaseMode() {
        keyMode = false
        pageTitle = i18n.tr("Available Databases")
        folderModel.nameFilters = ["*.kdbx"]
    }

    function onDatabaseSelected(index, model) {
        if(keyMode) {
            keyFilePath = model.filePath
            keyFileName = model.fileName            
            pageStack.clear()
            pageStack.push(openDatabase)
        } else {
            databaseFilePath = model.filePath            
            databaseFileName = model.fileName
            keyFilePath = ''
            keyFileName = ''
            pageStack.clear()
            pageStack.push(openDatabase)
        }

        sourcesView.currentIndex = index;
    }

    head {
        actions: [
            Action {
              iconName: "import"
              text: i18n.tr("Import")
              onTriggered: {
                pageStack.push(importer.pickerPage)
              }
            }]
    }

    title: i18n.tr(pageTitle)

    Column {

        spacing: units.gu(1)
        anchors {
            margins: units.gu(2)
            fill: parent
        }        

        FolderListModel {
            id: folderModel
            nameFilters: ["*.kdbx"]
            showDirs: false
            // I don't know yet how to make this get the correct folder for my app!
            folder: 'file:'+ appLocation            
        }

        UbuntuListView {
            id: sourcesView
            focus: true
            height: parent.height
            width: parent.width
            spacing: 5
            model: folderModel            
            delegate: ListItem {
                width: sourcesView.width
                height: units.gu(5)
                Text {
                    text: fileName
                    font.pointSize: 12
                    color: UbuntuColors.darkAubergine
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        onDatabaseSelected(index, model)
                    }
                }
                leadingActions: ListItemActions {
                    actions: [
                        Action {
                            iconName: "delete"
                            onTriggered: {                                
                                selectedDatabaseToDelete.path = folderModel.get(index, "filePath")
                                selectedDatabaseToDelete.name = folderModel.get(index, "fileName")
                                PopupUtils.open(deletePopup)
                            }
                        }
                    ]
                }
            }
        }
    }
}
