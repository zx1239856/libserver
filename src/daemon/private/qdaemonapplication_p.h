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

#ifndef QDAEMONAPPLICATIONPRIVATE_H
#define QDAEMONAPPLICATIONPRIVATE_H

#include "qdaemon-global.h"
#include "qdaemonlog.h"

#include <QtCore/qcommandlineparser.h>
#include <QtCore/qcommandlineoption.h>

QT_BEGIN_NAMESPACE

namespace QtDaemon
{
    class QAbstractDaemonBackend;
}

class QDaemonApplication;
class Q_DAEMON_EXPORT QDaemonApplicationPrivate
{
    Q_DECLARE_PUBLIC(QDaemonApplication)
public:
    QDaemonApplicationPrivate(QDaemonApplication *);
    ~QDaemonApplicationPrivate();

private:
    int exec();

    static void processSignalHandler(int);

private:
    QtDaemon::QAbstractDaemonBackend * createBackend(bool);

private:
    QDaemonApplication * q_ptr;
    QDaemonLog log;
    bool autoQuit;
    QCommandLineParser parser;

    static QString description;
};

QT_END_NAMESPACE

#endif // QDAEMONAPPLICATIONPRIVATE_H
