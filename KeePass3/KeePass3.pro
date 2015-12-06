TEMPLATE = app
TARGET = KeePass3

load(ubuntu-click)

QT += qml quick

SOURCES += main.cpp \    
    backend.cpp \
    compositekey.cpp \
    filesystem.cpp \
    hashedblockstream.cpp \
    passwordentry.cpp \
    readxmlfile.cpp \
    salsa20.cpp \
    sha256.cpp \
    tinyxml2.cpp

RESOURCES += KeePass3.qrc

LIBS += -L/usr/lib -lcryptopp

QML_FILES += $$files(*.qml,true) \
             $$files(*.js,true)

CONF_FILES +=  KeePass3.apparmor \
               KeePass3.png

AP_TEST_FILES += tests/autopilot/run \
                 $$files(tests/*.py,true)

#show all the files in QtCreator
OTHER_FILES += $${CONF_FILES} \
               $${QML_FILES} \
               $${AP_TEST_FILES} \
               KeePass3.desktop

#specify where the config files are installed to
config_files.path = /KeePass3
config_files.files += $${CONF_FILES}
INSTALLS+=config_files

#install the desktop file, a translated version is 
#automatically created in the build directory
desktop_file.path = /KeePass3
desktop_file.files = $$OUT_PWD/KeePass3.desktop
desktop_file.CONFIG += no_check_exist
INSTALLS+=desktop_file

# Default rules for deployment.
target.path = $${UBUNTU_CLICK_BINARY_PATH}
INSTALLS+=target

HEADERS += \
    backend.h \
    compositekey.h \
    filesystem.h \
    hashedblockstream.h \
    passwordentry.h \
    readxmlfile.h \
    salsa20.h \
    sha256.h \
    tinyxml2.h 

DISTFILES += \
    OpenDatabase.qml \
    Entry.qml \
    EntryModel.qml \
    ListEntryItems.qml
