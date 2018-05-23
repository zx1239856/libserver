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

#include "daemonbackend_linux.h"
#include "qdaemonapplication.h"
#include "qdaemonlog.h"

#include <QtCore/qstring.h>
#include <QtCore/qfileinfo.h>
#include <QtCore/qcommandlineparser.h>

#include <QtDBus/qdbusconnection.h>
#include <QtDBus/qdbuserror.h>

QT_BEGIN_NAMESPACE

using namespace QtDaemon;

DaemonBackendLinux::DaemonBackendLinux(QCommandLineParser & arguments)
    : QAbstractDaemonBackend(arguments)
{
}

DaemonBackendLinux::~DaemonBackendLinux()
{
}

int DaemonBackendLinux::exec()
{
    QString service = serviceName();

    // Connect to the DBus infrastructure
    QDBusConnection dbus = QDBusConnection::systemBus();
    if (!dbus.isConnected())  {
        qDaemonLog(QStringLiteral("Can't connect to the D-Bus system bus: %1").arg(dbus.lastError().message()), QDaemonLog::ErrorEntry);
        return BackendFailed;
    }

    // Register the service
    if (!dbus.registerService(service))  {
        qDaemonLog(QStringLiteral("Couldn't register a service with the D-Bus system bus: %1").arg(dbus.lastError().message()), QDaemonLog::ErrorEntry);
        return BackendFailed;
    }

    // Register the object
    if (!dbus.registerObject(QStringLiteral("/"), this, QDBusConnection::ExportAllInvokables))  {
        qDaemonLog(QStringLiteral("Couldn't register an object with the D-Bus system bus. (%1)").arg(dbus.lastError().message()), QDaemonLog::ErrorEntry);
        return BackendFailed;
    }

    QStringList arguments = parser.positionalArguments();
    arguments.prepend(QDaemonApplication::applicationFilePath());

    QMetaObject::invokeMethod(qApp, "daemonized", Qt::QueuedConnection, Q_ARG(QStringList, arguments));

    int status = QCoreApplication::exec();

    // Unregister the object
    dbus.unregisterObject(QStringLiteral("/"));

    // Unregister the service
    if (!dbus.unregisterService(service))
        qDaemonLog(QStringLiteral("Can't unregister service from D-bus. (%1)").arg(dbus.lastError().message()), QDaemonLog::WarningEntry);

    return status;
}

bool DaemonBackendLinux::isRunning()
{
    return true;	// This is just for notifying the controlling process. The function is invoked over D-Bus only.
}

bool DaemonBackendLinux::stop()
{
    qApp->quit();	// This is just to respond to the controlling process. The function is invoked over D-Bus only.
    return true;
}

QString DaemonBackendLinux::serviceName()
{
    QString executable = QFileInfo(QDaemonApplication::applicationFilePath()).completeBaseName();
    QString domain = QDaemonApplication::organizationDomain();

    // Get the service name
    if (domain.isEmpty())
        return QStringLiteral("io.qt.QtDaemon.%1").arg(executable);

    QStringList elements = domain.split('.', QString::SkipEmptyParts);
    std::reverse(elements.begin(), elements.end());
    return elements.join('.') + QStringLiteral(".") + executable;
}

QT_END_NAMESPACE
