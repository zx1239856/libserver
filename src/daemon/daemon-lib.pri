INCLUDEPATH += $$PWD

QT = core

#load(qt_build_config)
QMAKE_DOCS = $$PWD/doc/qtdaemon.qdocconf

DEFINES += QT_DAEMON_STATICLIB

unix:!macx  {
    QT += dbus
}

SOURCES += \
    $$PWD/qdaemonapplication.cpp \
    $$PWD/qdaemonlog.cpp \
    $$PWD/private/qdaemonlog_p.cpp \
    $$PWD/private/qdaemonapplication_p.cpp \
    $$PWD/private/qabstractdaemonbackend.cpp

PUBLIC_HEADERS += \
    $$PWD/qdaemon-global.h \
    $$PWD/qdaemonapplication.h \
    $$PWD/qdaemonlog.h

PRIVATE_HEADERS += \
    $$PWD/private/qdaemonapplication_p.h \
    $$PWD/private/qdaemonlog_p.h \
    $$PWD/private/qabstractdaemonbackend.h

unix:RESOURCES += $$PWD/qdaemon.qrc

macx {
    SOURCES += \
        $$PWD/private/controllerbackend_osx.cpp \
        $$PWD/private/daemonbackend_osx.cpp

    PRIVATE_HEADERS += \
        $$PWD/private/controllerbackend_osx.h \
        $$PWD/private/daemonbackend_osx.h
} else: unix {
    SOURCES += \
        $$PWD/private/controllerbackend_linux.cpp \
        $$PWD/private/daemonbackend_linux.cpp

    PRIVATE_HEADERS += \
        $$PWD/private/controllerbackend_linux.h \
        $$PWD/private/daemonbackend_linux.h


    #target.path = /usr/lib
    #INSTALLS += target

    DISTFILES += \
        resources/init \
        resources/dbus
} else: win32 {
    SOURCES += \
        $$PWD/private/controllerbackend_win.cpp \
        $$PWD/private/daemonbackend_win.cpp

    PRIVATE_HEADERS += \
        $$PWD/private/controllerbackend_win.h \
        $$PWD/private/daemonbackend_win.h

    LIBS += -luser32 -ladvapi32
}

HEADERS += $$PUBLIC_HEADERS $$PRIVATE_HEADERS
