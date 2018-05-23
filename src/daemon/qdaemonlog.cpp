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

#include "qdaemonlog.h"
#include "private/qdaemonlog_p.h"

#include <QtCore/QMutexLocker>

QT_BEGIN_NAMESPACE

/*!
    \class QDaemonLog
    \inmodule QtDaemon

    \brief The \l{QDaemonLog} class provides basic logging facilities for
    daemon applications.

    It's set up to output on the standard stream when the application is run as controlling terminal,
    and to a file when the application is ran as daemon/service.

    \threadsafe
*/

/*!
    \enum QDaemonLog::EntrySeverity

    This enum is used to specify the severity of the log message.

    \value NoticeEntry  The entry is a notice.
    \value WarningEntry The entry is a warning. Usually used when non-critical errors occur.
    \value ErrorEntry   The entry is an error. Usually used with critical errors.

    \sa qDaemonLog(const QString &, QDaemonLog::EntrySeverity)
*/

/*!
    \enum QDaemonLog::LogType

    This enum specifies the type of the log.

    \value LogToStdout  The messages are written to the standard output stream.
    \value LogToFile    The messages are written to a regular file.
                        The file is created in the application's directory if it doesn't exist.
                        The name of the file is constructed from the base name of the executable by appending a .log extension.
*/

/*!
    \internal
*/
QDaemonLog::QDaemonLog(QDaemonLogPrivate & d)
    : d_ptr(&d)
{
    Q_ASSERT(!QDaemonLogPrivate::logger);
    QDaemonLogPrivate::logger = this;
}

/*!
    \internal
*/
QDaemonLog::~QDaemonLog()
{
    QDaemonLogPrivate::logger = Q_NULLPTR;
    delete d_ptr;
}

/*!
    Sets the log type to \a type.

    \sa logType(), QDaemonLog::LogType
*/
void QDaemonLog::setLogType(LogType type)
{
    QMutexLocker lock(&d_ptr->streamMutex);
    Q_UNUSED(lock);

    if (type == d_ptr->logType)
        return;

    bool failed = false;
    switch (type)
    {
    case LogToFile:
        d_ptr->logFile.close();
        d_ptr->logFile.setFileName(d_ptr->logFilePath);
        // Try opening the file
        if (d_ptr->logFile.open(QFile::WriteOnly | QFile::Text | QFile::Append))  {
            d_ptr->logType = LogToFile;
            break;
        }

        // File couldn't be open. Try to fall back to the standard output
        failed = true;
    case LogToStdout:
    default:
        d_ptr->logFile.close();
        if (Q_UNLIKELY(!d_ptr->logFile.open(stdout, QFile::WriteOnly | QFile::Text)))  {
            qWarning("Error while trying to open the standard output. Giving up!");
            break;
        }

        d_ptr->logType = LogToStdout;
        if (failed)  // Report that a file couldn't be opened
            d_ptr->write(QStringLiteral("The log file %1 couldn't be opened for writing! Switched to stdout.").arg(d_ptr->logFilePath), WarningEntry);
    }
}

/*!
    Retrieves the currently used log type.

    \sa setLogType(), QDaemonLog::LogType
*/
QDaemonLog::LogType QDaemonLog::logType() const
{
    QMutexLocker lock(&d_ptr->streamMutex);	// The MS compiler doesn't get anonymous objects (error C2530: references must be initialized)
    Q_UNUSED(lock);							// Suppress warning for unused variable

    return d_ptr->logType;
}

/*!
    Writes the message specified by \a message to the log.

    \sa qDaemonLog()
*/
QDaemonLog & QDaemonLog::operator << (const QString & message)
{
    QMutexLocker lock(&d_ptr->streamMutex);	// The MS compiler doesn't get anonymous objects (error C2530: references must be initialized)
    Q_UNUSED(lock);                         // Suppress warning for unused variable

    d_ptr->write(message, QDaemonLog::NoticeEntry);
    return *this;
}

/*!
    \relates QDaemonLog

    Retrieves the logger instance.

    \warning The daemon log can be retrieved only after the QDaemonApplication instance has been created.

    \sa qDaemonLog(const QString &, QDaemonLog::EntrySeverity), QDaemonLog::operator<<()
*/
QDaemonLog & qDaemonLog()
{
    Q_ASSERT(QDaemonLogPrivate::logger);
    return *QDaemonLogPrivate::logger;
}

/*!
    \relates QDaemonLog
    \overload qDaemonLog()

    Writes a message specified by \a message to the log with a severity given by \a severity.

    \sa qDaemonLog(), QDaemonLog::operator<<()
*/
void qDaemonLog(const QString & message, QDaemonLog::EntrySeverity severity)
{
    Q_ASSERT(QDaemonLogPrivate::logger);

    QDaemonLogPrivate * const d = QDaemonLogPrivate::logger->d_ptr;

    QMutexLocker lock(&d->streamMutex); // The MS compiler doesn't get anonymous objects (error C2530: references must be initialized)
    Q_UNUSED(lock);                     // Suppress warning for unused variable

    d->write(message, severity);
}

QT_END_NAMESPACE
