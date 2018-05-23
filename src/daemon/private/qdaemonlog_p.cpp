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

#include "qdaemonlog_p.h"
#include "qdaemonlog.h"

#include <QtCore/qcoreapplication.h>
#include <QtCore/qdatetime.h>
#include <QtCore/qdir.h>
#include <QtCore/qfileinfo.h>

QT_BEGIN_NAMESPACE

QDaemonLog * QDaemonLogPrivate::logger = NULL;

QDaemonLogPrivate::QDaemonLogPrivate()
    : logStream(&logFile), logType(QDaemonLog::LogToStdout)
{
    // Get the log file path
    QFileInfo info(QCoreApplication::applicationFilePath());
    logFilePath = info.absoluteDir().filePath(info.completeBaseName() + QStringLiteral(".log"));

    // Open the default stdout logging
    if (Q_UNLIKELY(!logFile.open(stdout, QFile::WriteOnly | QFile::Text)))
        qWarning("Error while trying to open the standard output. Giving up!");
}

QDaemonLogPrivate::~QDaemonLogPrivate()
{
    logStream.flush();
    logFile.close();
}

void QDaemonLogPrivate::write(const QString & message, QDaemonLog::EntrySeverity severity)
{
    static const QString noticeEntry = QStringLiteral("%1 %2");
    static const QString warningEntry = QStringLiteral("%1 Warning: %2");
    static const QString errorEntry = QStringLiteral("%1 Error: %2");

    QString formattedMessage, date = QDateTime::currentDateTime().toString(Qt::ISODate);
    switch (severity)
    {
    case QDaemonLog::ErrorEntry:
        formattedMessage = errorEntry.arg(date).arg(message);
        break;
    case QDaemonLog::WarningEntry:
        formattedMessage = warningEntry.arg(date).arg(message);
        break;
    case QDaemonLog::NoticeEntry:
    default:
        formattedMessage = noticeEntry.arg(date).arg(message);
    }

    logStream << formattedMessage << endl;
}

QT_END_NAMESPACE
