include(daemon/daemon-lib.pri)
include(libserver.pri)
include(qcron/qcron.pri)

QMAKE_CXXFLAGS += -Wimplicit-fallthrough=0 -Wswitch

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
    utils/crypto/token.h \
    utils/file/imghandler.h \
    utils/smtp/emailaddress.h \
    utils/smtp/mimeattachment.h \
    utils/smtp/mimecontentformatter.h \
    utils/smtp/mimefile.h \
    utils/smtp/mimehtml.h \
    utils/smtp/mimeinlinefile.h \
    utils/smtp/mimemessage.h \
    utils/smtp/mimemultipart.h \
    utils/smtp/mimepart.h \
    utils/smtp/mimetext.h \
    utils/smtp/quotedprintable.h \
    utils/smtp/smtpclient.h \
    utils/smtp/smtpexports.h \
    utils/smtp/SmtpMime \
    utils/smtp/sendemail.h \
    utils/dbwrapper/sqlarray.h \
    utils/file/tcpfiletransfer.h \
    utils/worker/bgworker.h \
    utils/worker/bgworkercontroller.h \
    utils/handle/dblog.h \
    utils/handle/dbsettings.h

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
    utils/crypto/token.cpp \
    utils/file/imghandler.cpp \
    utils/smtp/emailaddress.cpp \
    utils/smtp/mimeattachment.cpp \
    utils/smtp/mimecontentformatter.cpp \
    utils/smtp/mimefile.cpp \
    utils/smtp/mimehtml.cpp \
    utils/smtp/mimeinlinefile.cpp \
    utils/smtp/mimemessage.cpp \
    utils/smtp/mimemultipart.cpp \
    utils/smtp/mimepart.cpp \
    utils/smtp/mimetext.cpp \
    utils/smtp/quotedprintable.cpp \
    utils/smtp/smtpclient.cpp \
    utils/smtp/sendemail.cpp \
    utils/dbwrapper/sqlarray.cpp \
    utils/file/tcpfiletransfer.cpp \
    utils/worker/bgworker.cpp \
    utils/worker/bgworkercontroller.cpp \
    utils/handle/dblog.cpp \
    utils/handle/dbsettings.cpp

unix:!macx  {
    HEADERS += \
    epoll/eventdispatcher_epoll.h \
    epoll/eventdispatcher_epoll_p.h \
    epoll/qt4compat.h

    SOURCES += \
    epoll/eventdispatcher_epoll.cpp \
    epoll/eventdispatcher_epoll_p.cpp \
    epoll/socknot_p.cpp \
    epoll/timers_p.cpp

}
