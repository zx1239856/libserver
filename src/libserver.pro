include(daemon/daemon-lib.pri)
include(libserver.pri)

QMAKE_CXXFLAGS += -Wimplicit-fallthrough=0

DEFINES += VERBOSE_OUTPUT

TARGET = libserver

HEADERS += \
    utils/dbwrapper/db-operation.h \
    utils/dbwrapper/db-wrapper.h \
    utils/file/fileio.h \
    utils/file/pdfhandler.h \
    utils/handle/controlhdl.h \
    utils/handle/handle.h \
    utils/handle/userhdl.h \
    utils/web/requesthdl.h \
    utils/web/socketthread.h \
    utils/web/webserver.h \
    utils/command.h \
    utils/config.h \
    utils/exception.h \
    utils/cmdparser.h \
    utils/mainservice.h \
    globalInfo.h \
    utils/handle/queryhdl.h \
    utils/handle/appointhdl.h \
    utils/handle/checkhdl.h \
    utils/handle/operateuserhdl.h \
    utils/handle/operategrouphdl.h \
    utils/handle/operatebookhdl.h \
    utils/cryto/token.h

SOURCES += \
    utils/dbwrapper/db-operation.cpp \
    utils/dbwrapper/db-wrapper.cpp \
    utils/file/fileio.cpp \
    utils/file/pdfhandler.cpp \
    utils/handle/controlhdl.cpp \
    utils/handle/handle.cpp \
    utils/handle/userhdl.cpp \
    utils/web/requesthdl.cpp \
    utils/web/socketthread.cpp \
    utils/web/webserver.cpp \
    utils/config.cpp \
    main.cpp \
    utils/cmdparser.cpp \
    utils/mainservice.cpp \
    utils/handle/queryhdl.cpp \
    utils/handle/appointhdl.cpp \
    utils/handle/checkhdl.cpp \
    utils/handle/operateuserhdl.cpp \
    utils/handle/operategrouphdl.cpp \
    utils/handle/operatebookhdl.cpp \
    utils/cryto/token.cpp
