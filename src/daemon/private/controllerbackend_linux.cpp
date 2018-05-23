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

#include "controllerbackend_linux.h"
#include "daemonbackend_linux.h"
#include "qdaemonapplication.h"
#include "qdaemonlog.h"

#include <QtCore/qmetaobject.h>
#include <QtCore/qcommandlineparser.h>
#include <QtCore/qcommandlineoption.h>
#include <QtCore/qtextstream.h>
#include <QtCore/qprocess.h>
#include <QtCore/qthread.h>
#include <QtCore/qdir.h>
#include <QtCore/qfile.h>
#include <QtCore/qfileinfo.h>
#include <QtCore/qelapsedtimer.h>

#include <QtDBus/qdbusconnection.h>
#include <QtDBus/qdbuserror.h>
#include <QtDBus/qdbusinterface.h>
#include <QtDBus/qdbusreply.h>

QT_BEGIN_NAMESPACE

using namespace QtDaemon;

static qint32 dbusServiceTimeout = 30000;		// Up to 30 seconds
static qint32 dbusPollTime = 1000;				// Poll each second on start

const QString ControllerBackendLinux::initdPrefix = QStringLiteral("initd-prefix");
const QString ControllerBackendLinux::dbusPrefix = QStringLiteral("dbus-prefix");
const QString ControllerBackendLinux::defaultInitPath = QStringLiteral("/etc/init.d");
const QString ControllerBackendLinux::defaultDBusPath = QStringLiteral("/etc/dbus-1/system.d");

ControllerBackendLinux::ControllerBackendLinux(QCommandLineParser & parser, bool autoQuit)
    : QAbstractControllerBackend(parser, autoQuit),
      dbusPrefixOption(dbusPrefix, QCoreApplication::translate("main", "Sets the path for the installed dbus configuration file"), QStringLiteral("path"), defaultDBusPath),
      initdPrefixOption(initdPrefix, QCoreApplication::translate("main", "Sets the path for the installed init.d script"), QStringLiteral("path"), defaultInitPath)
{
    parser.addOption(dbusPrefixOption);
    parser.addOption(initdPrefixOption);
}

bool ControllerBackendLinux::start()
{
    // Connect to the DBus infrastructure
    QDBusConnection dbus = QDBusConnection::systemBus();
    if (!dbus.isConnected())  {
        qDaemonLog(QStringLiteral("Can't connect to the DBus system bus (%1)").arg(dbus.lastError().message()), QDaemonLog::ErrorEntry);
        return false;
    }

    // Get the service name
    QString service = DaemonBackendLinux::serviceName();

    // First check if the daemon is already running
    QScopedPointer<QDBusAbstractInterface> interface(new QDBusInterface(service, QStringLiteral("/"), QStringLiteral(Q_DAEMON_DBUS_CONTROL_INTERFACE), dbus));
    if (interface->isValid())  {
        QDBusReply<bool> reply = interface->call(QStringLiteral("isRunning"));
        if (reply.isValid() && reply.value())
            qDaemonLog(QStringLiteral("The daemon is already running."), QDaemonLog::NoticeEntry);
        else
            qDaemonLog(QStringLiteral("The daemon is not responding."), QDaemonLog::ErrorEntry);

        return false;
    }

    // The daemon is (most probably) not running, so start it with the proper arguments
    QStringList arguments = parser.positionalArguments();
    //if (arguments.size() > 0)
    //   arguments.prepend(QStringLiteral("--"));
    arguments.prepend(QStringLiteral("-d"));
    if (!QProcess::startDetached(QDaemonApplication::applicationFilePath(), arguments, QDaemonApplication::applicationDirPath()))  {
        qDaemonLog(QStringLiteral("The daemon failed to start."), QDaemonLog::ErrorEntry);
        return false;
    }

    // Repeat the call to make sure the communication is ok
    QElapsedTimer dbusTimeoutTimer;
    dbusTimeoutTimer.start();

    // Give the daemon some seconds to start its DBus service
    while (!dbusTimeoutTimer.hasExpired(dbusServiceTimeout))  {
        interface.reset(new QDBusInterface(service, QStringLiteral("/"), QStringLiteral(Q_DAEMON_DBUS_CONTROL_INTERFACE), dbus));
        if (interface->isValid())
            break;

        QThread::msleep(dbusPollTime);	// Wait some time before retrying
    }

    // Check the DBus status
    if (!interface)  {
        qDaemonLog(QStringLiteral("Connection with the daemon couldn't be established. (%1)").arg(dbus.lastError().message()), QDaemonLog::ErrorEntry);
        return false;
    }

    QDBusReply<bool> reply = interface->call(QStringLiteral("isRunning"));
    if (!reply.isValid() || !reply.value())  {
        qDaemonLog(QStringLiteral("The acquired DBus interface replied erroneously. (%1)").arg(dbus.lastError().message()), QDaemonLog::ErrorEntry);
        return false;
    }

    QMetaObject::invokeMethod(qApp, "started", Qt::QueuedConnection);
    return true;
}

bool ControllerBackendLinux::stop()
{
    // Connect to the DBus infrastructure
    QDBusConnection dbus = QDBusConnection::systemBus();
    if (!dbus.isConnected())  {
        qDaemonLog(QStringLiteral("Can't connect to the DBus system bus (%1)").arg(dbus.lastError().message()), QDaemonLog::ErrorEntry);
        return false;
    }

    // Get the service name
    QString service = DaemonBackendLinux::serviceName();

    // Acquire the DBus interface
    QScopedPointer<QDBusAbstractInterface> interface(new QDBusInterface(service, QStringLiteral("/"), QStringLiteral(Q_DAEMON_DBUS_CONTROL_INTERFACE), dbus));
    if (!interface->isValid())  {
        qDaemonLog(QStringLiteral("Couldn't acquire the DBus interface. Is the daemon running? (%1)").arg(dbus.lastError().message()), QDaemonLog::ErrorEntry);
        return false;
    }

    QDBusReply<bool> reply = interface->call(QStringLiteral("stop"));
    if (!reply.isValid() || !reply.value())  {
        qDaemonLog(QStringLiteral("The acquired DBus interface replied erroneously. (%1)").arg(dbus.lastError().message()), QDaemonLog::ErrorEntry);
        return false;
    }

    QMetaObject::invokeMethod(qApp, "stopped", Qt::QueuedConnection);
    return true;
}

bool ControllerBackendLinux::install()
{
    QFileInfo applicationInfo(QDaemonApplication::applicationFilePath());
    QString path = applicationInfo.absoluteFilePath(), executable = applicationInfo.fileName(), service = DaemonBackendLinux::serviceName();

    QString dbusPath = parser.isSet(dbusPrefixOption) ? parser.value(dbusPrefixOption) : defaultDBusPath;
    QString initdPath = parser.isSet(initdPrefixOption) ? parser.value(initdPrefixOption) : defaultInitPath;

    // Sanity check for the paths and permissions of the provided directories
    if (dbusPath.isEmpty() || initdPath.isEmpty())  {
        qDaemonLog(QStringLiteral("The provided D-Bus path and/or init.d path can't be empty"), QDaemonLog::ErrorEntry);
        return false;
    }

    // Make those directories absolute (it matters when running uninstall)
    dbusPath = QDir(dbusPath).absolutePath();
    initdPath = QDir(initdPath).absolutePath();

    QString dbusFilePath = QDir(dbusPath).filePath(service + QStringLiteral(".conf"));
    QString initdFilePath = QDir(initdPath).filePath(executable);

    QFile dbusConf(dbusFilePath), initdFile(initdFilePath);
    if (dbusConf.exists())  {
        qDaemonLog(QStringLiteral("The provided D-Bus configuration directory already contains a configuration for this service. Uninstall first"), QDaemonLog::ErrorEntry);
        return false;
    }
    if (initdFile.exists())  {
        qDaemonLog(QStringLiteral("The provided init.d directory already contains a script for this service. Uninstall first"), QDaemonLog::ErrorEntry);
        return false;
    }

    if (!dbusConf.open(QFile::WriteOnly | QFile::Text))  {
        qDaemonLog(QStringLiteral("Couldn't open the D-Bus configuration file for writing (%1).").arg(dbusFilePath), QDaemonLog::ErrorEntry);
        return false;
    }

    if (!initdFile.open(QFile::WriteOnly | QFile::Text))  {
        qDaemonLog(QStringLiteral("Couldn't open the init.d script for writing (%1).").arg(initdFilePath), QDaemonLog::ErrorEntry);
        dbusConf.remove();		// Remove the created dbus configuration
        return false;
    }

    // We have opened both files, read the templates
    QFile dbusTemplate(QStringLiteral(":/resources/dbus")), initdTemplate(QStringLiteral(":/resources/init"));

    // We don't expect resources to be inaccessible, but who knows ...
    if (!dbusTemplate.open(QFile::ReadOnly | QFile::Text) || !initdTemplate.open(QFile::ReadOnly | QFile::Text))  {
        qDaemonLog(QStringLiteral("Couldn't read the daemon's resources!"), QDaemonLog::ErrorEntry);
        return false;
    }

    // Read the dbus configuration, do the substitution and write to disk
    QTextStream fin(&dbusTemplate), fout(&dbusConf);
    QString data = fin.readAll();
    data.replace(QStringLiteral("%%SERVICE_NAME%%"), service);
    fout << data;

    if (fout.status() != QTextStream::Ok)  {
        qDaemonLog(QStringLiteral("An error occured while writing the D-Bus configuration. Installation may be broken."), QDaemonLog::WarningEntry);
        fout.resetStatus();
    }

    // Set the permissions for the dbus configuration
    if (!dbusConf.setPermissions(QFile::WriteOwner | QFile::ReadOwner | QFile::ReadGroup | QFile::ReadOther))
        qDaemonLog(QStringLiteral("An error occured while setting the permissions for the D-Bus configuration. Installation may be broken"), QDaemonLog::WarningEntry);

    // Switch IO devices
    fin.setDevice(&initdTemplate);
    fout.setDevice(&initdFile);

    // Read the init.d script, do the substitution and write to disk
    QStringList arguments = parser.positionalArguments();
    if (arguments.size() > 0)
        arguments.prepend(QStringLiteral("--"));

    data = fin.readAll();
    data.replace(QStringLiteral("%%DAEMON%%"), path)
        .replace(QStringLiteral("%%EXECUTABLE%%"), executable)
        .replace(QStringLiteral("%%NAME%%"), QDaemonApplication::applicationName())
        .replace(QStringLiteral("%%DESCRIPTION%%"), QDaemonApplication::applicationDescription())
        .replace(QStringLiteral("%%INITD_PREFIX%%"), initdPath)
        .replace(QStringLiteral("%%DBUS_PREFIX%%"), dbusPath)
        .replace(QStringLiteral("%%ARGUMENTS%%"), arguments.join(' '));
    fout << data;

    if (fout.status() != QTextStream::Ok)
        qDaemonLog(QStringLiteral("An error occured while writing the init.d script. Installation may be broken."), QDaemonLog::WarningEntry);

    // Set the permissions for the init.d script
    if (!initdFile.setPermissions(QFile::WriteOwner | QFile::ExeOwner | QFile::ReadOwner | QFile::ReadGroup | QFile::ExeGroup | QFile::ReadOther | QFile::ExeOther))
        qDaemonLog(QStringLiteral("An error occured while setting the permissions for the init.d script. Installation may be broken."), QDaemonLog::WarningEntry);

    QMetaObject::invokeMethod(qApp, "installed", Qt::QueuedConnection);
    return true;
}

bool ControllerBackendLinux::uninstall()
{
    QFileInfo applicationInfo(QDaemonApplication::applicationFilePath());
    QString executable = applicationInfo.fileName(), service = DaemonBackendLinux::serviceName();

    QString dbusPath = parser.isSet(dbusPrefixOption) ? parser.value(dbusPrefixOption) : defaultDBusPath;
    QString initdPath = parser.isSet(initdPrefixOption) ? parser.value(initdPrefixOption) : defaultInitPath;

    dbusPath = QDir(dbusPath).absolutePath();
    initdPath = QDir(initdPath).absolutePath();

    QString dbusFilePath = QDir(dbusPath).filePath(service + QStringLiteral(".conf"));
    QString initdFilePath = QDir(initdPath).filePath(executable);

    QFile dbusConf(dbusFilePath), initdFile(initdFilePath);
    if (dbusConf.exists() && !dbusConf.remove())  {
        qDaemonLog(QStringLiteral("Couldn't remove the D-Bus configuration file for this service (%1).").arg(dbusFilePath), QDaemonLog::ErrorEntry);
        return false;
    }
    if (initdFile.exists() && !initdFile.remove())  {
        qDaemonLog(QStringLiteral("Couldn't remove the init.d script for this service (%1).").arg(initdFilePath), QDaemonLog::ErrorEntry);
        return false;
    }

    QMetaObject::invokeMethod(qApp, "uninstalled", Qt::QueuedConnection);
    return true;
}

QAbstractControllerBackend::DaemonStatus ControllerBackendLinux::status()
{
    // Connect to the DBus infrastructure
    QDBusConnection dbus = QDBusConnection::systemBus();
    if (!dbus.isConnected())  {
        qDaemonLog(QStringLiteral("Can't connect to the DBus system bus (%1)").arg(dbus.lastError().message()), QDaemonLog::ErrorEntry);
        return NotRunningStatus;
    }

    // Get the service name
    QString service = DaemonBackendLinux::serviceName();

    // Acquire the DBus interface
    QScopedPointer<QDBusAbstractInterface> interface(new QDBusInterface(service, QStringLiteral("/"), QStringLiteral(Q_DAEMON_DBUS_CONTROL_INTERFACE), dbus));
    if (!interface->isValid())
        return NotRunningStatus;

    QDBusReply<bool> reply = interface->call(QStringLiteral("isRunning"));
    return reply.isValid() && reply.value() ? RunningStatus : NotRunningStatus;
}

QT_END_NAMESPACE
