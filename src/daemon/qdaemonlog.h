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

#ifndef QDAEMONLOG_H
#define QDAEMONLOG_H

#include "qdaemon-global.h"

QT_BEGIN_NAMESPACE

class QDaemonLogPrivate;
class Q_DAEMON_EXPORT QDaemonLog
{
    Q_DISABLE_COPY(QDaemonLog)

public:
    enum EntrySeverity  { NoticeEntry, WarningEntry, ErrorEntry };
    enum LogType { LogToStdout, LogToFile };

    QDaemonLog(QDaemonLogPrivate &);
    ~QDaemonLog();

    void setLogType(LogType type);
    LogType logType() const;

    QDaemonLog & operator << (const QString & message);

    friend Q_DAEMON_EXPORT QDaemonLog & qDaemonLog();
    friend Q_DAEMON_EXPORT void qDaemonLog(const QString & message, QDaemonLog::EntrySeverity severity);

private:
    QDaemonLogPrivate * d_ptr;
};

// --- Friend declarations ---------------------------------------------------------------------------------------------- //
Q_DAEMON_EXPORT QDaemonLog & qDaemonLog();
Q_DAEMON_EXPORT void qDaemonLog(const QString & message, QDaemonLog::EntrySeverity severity = QDaemonLog::NoticeEntry);
// ---------------------------------------------------------------------------------------------------------------------- //

QT_END_NAMESPACE

#endif // QDAEMONLOG_H
