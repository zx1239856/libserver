QT += core
CONFIG += c++1

HEADERS += $$PWD/holiday.h \
           $$PWD/qcron.h \
           $$PWD/qcronfield.h \
           $$PWD/qcronnode.h
SOURCES += $$PWD/holiday.cpp \
           $$PWD/qcron.cpp \
           $$PWD/qcronfield.cpp \
           $$PWD/qcronnode.cpp

INCLUDEPATH += $$PWD

DISTFILES += $$PWD/../README.md \
             $$PWD/../LICENSE
