/****************************************************************************
**
** Copyright (C) 2016 Konstantin Shegunov <kshegunov@gmail.com>
**
** This file is part of the QtDaemon library.
**
** The MIT License (MIT)
**
** Permission is hereby granted, free of charge, to any person obtaining a copy
** of this software and associated documentation files (the "Software"), to deal
** in the Software without restriction, including without limitation the rights
** to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
** copies of the Software, and to permit persons to whom the Software is
** furnished to do so, subject to the following conditions:
**
** The above copyright notice and this permission notice shall be included in all
** copies or substantial portions of the Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
** OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
** SOFTWARE.
**
****************************************************************************/

//
//  W A R N I N G
//  -------------
//
// This file is not part of the QtDaemon API. It exists only
// as an implementation detail. This header file may change from
// version to version without notice, or even be removed.
//
// We mean it.
//

#ifndef CONTROLLERBACKEND_LINUX_H
#define CONTROLLERBACKEND_LINUX_H

#include "qabstractdaemonbackend.h"

QT_BEGIN_NAMESPACE

class QDBusAbstractInterface;

namespace QtDaemon
{
    class Q_DAEMON_LOCAL ControllerBackendLinux : public QAbstractControllerBackend
    {
        Q_DISABLE_COPY(ControllerBackendLinux)

    public:
        ControllerBackendLinux(QCommandLineParser &, bool);

        bool start() Q_DECL_OVERRIDE;
        bool stop() Q_DECL_OVERRIDE;
        bool install() Q_DECL_OVERRIDE;
        bool uninstall() Q_DECL_OVERRIDE;
        DaemonStatus status() Q_DECL_OVERRIDE;

    private:
        QDBusAbstractInterface * getDBusInterface();

        const QCommandLineOption dbusPrefixOption;
        const QCommandLineOption initdPrefixOption;

        static const QString initdPrefix;
        static const QString dbusPrefix;
        static const QString defaultInitPath;
        static const QString defaultDBusPath;
    };
}

QT_END_NAMESPACE

#endif // CONTROLLERBACKEND_LINUX_H
