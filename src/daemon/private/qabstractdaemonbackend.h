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

#ifndef QABSTRACTDAEMONBACKEND_H
#define QABSTRACTDAEMONBACKEND_H

#include "qdaemon-global.h"

#include <QtCore/qstringlist.h>
#include <QtCore/qcommandlineoption.h>

QT_BEGIN_NAMESPACE

class QCommandLineParser;

namespace QtDaemon
{
    class Q_DAEMON_LOCAL QAbstractDaemonBackend
    {
        Q_DISABLE_COPY(QAbstractDaemonBackend)

    public:
        static const int BackendFailed;

    public:
        QAbstractDaemonBackend(QCommandLineParser &);
        virtual ~QAbstractDaemonBackend();

        virtual int exec() = 0;

    protected:
        QCommandLineParser & parser;
    };

    class Q_DAEMON_LOCAL QAbstractControllerBackend : public QAbstractDaemonBackend
    {
        Q_DISABLE_COPY(QAbstractControllerBackend)

    protected:
        enum DaemonStatus {
            RunningStatus,
            NotRunningStatus
        };

    public:
        QAbstractControllerBackend(QCommandLineParser &, bool);

        int exec() Q_DECL_OVERRIDE;

        virtual bool start() = 0;
        virtual bool stop() = 0;
        virtual bool install() = 0;
        virtual bool uninstall() = 0;
        virtual DaemonStatus status() = 0;

    protected:
        bool autoQuit;

        const QCommandLineOption installOption;
        const QCommandLineOption uninstallOption;
        const QCommandLineOption startOption;
        const QCommandLineOption stopOption;
        const QCommandLineOption statusOption;
        const QCommandLineOption fakeOption;
    };
}

QT_END_NAMESPACE

#endif // QABSTRACTDAEMONBACKEND_H
