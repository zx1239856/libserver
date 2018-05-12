QT -= gui
QT += sql concurrent network

CONFIG += c++11 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += main.cpp \
    utils/dbwrapper/db-wrapper.cpp \
    utils/dbwrapper/db-operation.cpp \
    utils/config.cpp \
    utils/file/fileio.cpp \
    utils/file/logstream.cpp \
    utils/web/webserver.cpp \
    utils/web/socketthread.cpp \
    utils/web/requesthdl.cpp

HEADERS += \
    utils/dbwrapper/db-wrapper.h \
    utils/dbwrapper/db-operation.h \
    utils/config.h \
    utils/file/fileio.h \
    utils/file/logstream.h \
    utils/web/webserver.h \
    utils/web/socketthread.h \
    utils/web/requesthdl.h
