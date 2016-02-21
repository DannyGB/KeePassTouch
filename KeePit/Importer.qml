import QtQuick 2.0
import Ubuntu.Components 1.2
import Ubuntu.Components.Popups 1.2
import Ubuntu.Components.ListItems 1.0
import Ubuntu.Content 1.1

Item {
    id: importer
    property var contentStore: null
    property var activeTransfer: null
    property var pickerPage: picker
    property var importState: { "new": 1, "processing": 2, "imported": 3, "exists": 4, "error": 5 }
    property bool importing: false

    Connections {
        target: ContentHub
        onImportRequested: {
            activeTransfer = transfer            
            if (activeTransfer.state === ContentTransfer.Charged)
                importItems(activeTransfer.items)
        }
    }

    Connections {
        target: activeTransfer
        onStateChanged: {
            if (activeTransfer.state === ContentTransfer.Charged)
                importItems(activeTransfer.items)
        }
    }

    function doImport(filename, item) {
        return function () {
            var components = filename.split("/").pop().split(".")
            console.info(components)
            var ext = components.pop()
            console.info(ext)
            var dir = appLocation
            var basename =components.join(".")
            console.info(basename)
            var newfilename = basename + "." + ext
            console.info(newfilename)
            //item.item.move(dir, newfilename)
            item.state = importState.imported
        }
    }

    function importItems(items) {
        pageStack.push(importPage)
        importing = true
        for (var i=0; i<items.length; i++) {
            itemList.append({ "item": items[i], "state": importState.new, "error": "", "importName": "" })
        }
        importTimer.start()
    }

    function clearAndLoad(item) {
        if (item.importName != "") {
            var name = item.importName
            itemList.clear()
            loadFile(name)
        }
    }

    Timer {
        id: importTimer
        interval: 10
        onTriggered: {
            for (var i=0; i<itemList.count; i++) {
                var item = itemList.get(i)
                if (item.state == importState.new) {
                    var filename = item.item.url.toString().slice(7)
                    console.info("filename: " + filename)
                    doImport(filename, item)()
                }
            }
            if (i == itemList.count) {
                importing = false
                if (itemList.count == 1)
                    clearAndLoad(itemList.get(0))
            } else {
                importTimer.start()
            }
        }
    }

    ListModel {
        id: itemList
    }

    Page {
        id: importPage
        visible: false
        title: i18n.tr("Importing database...")
        head.backAction: Action {
            iconName: importing ? "preferences-system-updates-symbolic" : "back"
            onTriggered: {
                if (!importing) {
                    itemList.clear()
                    pageStack.pop()
                }
            }
        }

        ListView {
            id: sourcesView
            anchors.fill: parent

            model: itemList
            delegate: Subtitled {
                text: model.item.url.toString().split("/").pop()
                subText: {
                    switch (model.state) {
                    case importState.new:
                        return i18n.tr("Waiting")
                    case importState.processing:
                        return i18n.tr("Processing")
                    case importState.imported:
                        return i18n.tr("Imported to %1").arg(model.importName)
                    case importState.exists:
                        return i18n.tr("Already in library: %1").arg(model.importName)
                    case importState.error:
                        return i18n.tr("Error: %1").arg(model.error.split("\n\n")[0])
                    }
                }
                onClicked: {
                    if (!importing)
                        clearAndLoad(model)
                }
            }
        }
        Scrollbar {
            flickableItem: sourcesView
            align: Qt.AlignTrailing
        }
    }

    Page {
        id: picker
        visible: false
        ContentPeerPicker {
            handler: ContentHandler.Source
            contentType: ContentType.Documents
            headerText: i18n.tr("Import database from")

            onPeerSelected: {
                peer.selectionType = ContentTransfer.Multiple
                activeTransfer = peer.request(appStore)
                pageStack.pop()
            }

            onCancelPressed: pageStack.pop()
        }
    }

    ContentTransferHint {
        activeTransfer: importer.activeTransfer
    }

    ContentStore {
        id: appStore
        scope: ContentScope.App
    }
}
