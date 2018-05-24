#ifndef QDAEMON_GLOBAL_H
#define QDAEMON_GLOBAL_H

#include <QtGlobal>

QT_BEGIN_NAMESPACE

#if !defined(QT_DAEMON_STATICLIB)
    #if defined(QT_BUILD_DAEMON_LIB)
        #define Q_DAEMON_EXPORT Q_DECL_EXPORT
        #define Q_DAEMON_LOCAL Q_DECL_HIDDEN
    #else
        #define Q_DAEMON_EXPORT Q_DECL_IMPORT
    #endif
#else
    #define Q_DAEMON_EXPORT
    #define Q_DAEMON_LOCAL
#endif

QT_END_NAMESPACE

#endif // QDAEMON_GLOBAL_H
