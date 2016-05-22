#-------------------------------------------------
#
# Project created by QtCreator 2016-05-22T13:32:00
#
#-------------------------------------------------

load(ubuntu-click)

QT       -= core gui

TARGET = ziplib
TEMPLATE = lib
CONFIG += staticlib

SOURCES += \
    adler32.c \
    compress.c \
    crc32.c \
    deflate.c \
    gzio.c \
    infblock.c \
    infcodes.c \
    inffast.c \
    inflate.c \
    inftrees.c \
    infutil.c \
    maketree.c \
    trees.c \
    uncompr.c \
    zutil.c

HEADERS += \
    deflate.h \
    infblock.h \
    infcodes.h \
    inffast.h \
    inffixed.h \
    inftrees.h \
    infutil.h \
    trees.h \
    zconf.h \
    zlib.h \
    zutil.h \
    GZipHelper.h
unix {
    target.path = $${UBUNTU_CLICK_BINARY_PATH}
    INSTALLS += target
}
