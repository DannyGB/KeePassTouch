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
            pageStack.pop()
        } else {
            pageStack.push(openDatabase)
            databaseFilePath = model.filePath
            console.log(model.filePath)
            databaseFileName = model.fileName
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

        Component {
            id: fileDelegate
            Item {
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
            }
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
            delegate: fileDelegate
        }
    }
}
