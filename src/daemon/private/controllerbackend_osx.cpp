/****************************************************************************
**
** Copyright (C) 2016 Samuel Gaist <samuel.gaist@edeltech.ch>
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

#include "controllerbackend_osx.h"
#include "daemonbackend_osx.h"
#include "qdaemonapplication.h"
#include "qdaemonlog.h"

#include <QtCore/qcommandlineparser.h>
#include <QtCore/qcommandlineoption.h>
#include <QtCore/qsettings.h>
#include <QtCore/qfileinfo.h>
#include <QtCore/qdir.h>
#include <QtCore/qprocess.h>
#include <QtCore/qregularexpression.h>

QT_BEGIN_NAMESPACE

using namespace QtDaemon;

static const QString kUserAgentsPath = QStringLiteral("%1/Library/LaunchAgents"); //Per-user agents provided by the user.
static const QString kSystemAgentsPath = QStringLiteral("/Library/LaunchAgents");    //Per-user agents provided by the administrator.
static const QString kDaemonsPath = QStringLiteral("/Library/LaunchDaemons");    //System-wide daemons provided by the administrator.
static const QString kPlistFilePathKey = QStringLiteral("plistfilepath");

ControllerBackendOSX::ControllerBackendOSX(QCommandLineParser & parser, bool autoQuit)
    : QAbstractControllerBackend(parser, autoQuit),
      agentOption(QStringLiteral("agent"), QCoreApplication::translate("main", "Sets the daemon as an agent")),
      userOption(QStringLiteral("user"), QCoreApplication::translate("main", "Sets the agent as user local (affects only the agent option)"))
{
    parser.addOption(agentOption);
    parser.addOption(userOption);
}

bool ControllerBackendOSX::start()
{
    QSettings settings;
    const QString daemonConfigurationFilePath = settings.value(kPlistFilePathKey, QString()).toString();

    QProcess launchctl;
    launchctl.start(QStringLiteral("launchctl"), QStringList() << QStringLiteral("load") << daemonConfigurationFilePath);
    bool success = launchctl.waitForStarted();

    if (!success) {
        qDaemonLog(QStringLiteral("Couldn't start the daemon %1").arg(QString::fromUtf8(launchctl.readAllStandardError())), QDaemonLog::ErrorEntry);
        return false;
    }

    success = launchctl.waitForFinished();

    if (success)
        QMetaObject::invokeMethod(qApp, "started", Qt::QueuedConnection);
    else
        qDaemonLog(QStringLiteral("Couldn't start the daemon %1").arg(QString::fromUtf8(launchctl.readAllStandardError())), QDaemonLog::ErrorEntry);

    return success;
}

bool ControllerBackendOSX::stop()
{
    QSettings settings;
    const QString daemonConfigurationFilePath = settings.value(kPlistFilePathKey, QString()).toString();

    QProcess launchctl;
    launchctl.start(QStringLiteral("launchctl"), QStringList() << QStringLiteral("unload") << daemonConfigurationFilePath);
    bool success = launchctl.waitForStarted();

    if (!success) {
        qDaemonLog(QStringLiteral("Couldn't stop the daemon %1").arg(QString::fromUtf8(launchctl.readAllStandardError())), QDaemonLog::ErrorEntry);
        return false;
    }

    success = launchctl.waitForFinished();

    if (success)
        QMetaObject::invokeMethod(qApp, "stopped", Qt::QueuedConnection);
    else
        qDaemonLog(QStringLiteral("Couldn't stop the daemon %1").arg(QString::fromUtf8(launchctl.readAllStandardError())), QDaemonLog::ErrorEntry);

    return success;
}

bool ControllerBackendOSX::install()
{
    QFile plistTemplate(QStringLiteral(":/resources/plist"));
    if (!plistTemplate.open(QFile::ReadOnly | QFile::Text))  {
        qDaemonLog(QStringLiteral("Couldn't read the daemon's resources!"), QDaemonLog::ErrorEntry);
        return false;
    }

    QString daemonConfigurationFilePath = configurationFilePath();

    QFile plistOutFile(daemonConfigurationFilePath);
    if (!plistOutFile.open(QFile::WriteOnly | QFile::Text))  {
        qDaemonLog(QStringLiteral("Couldn't open output file. Check your permissions for %1!").arg(daemonConfigurationFilePath), QDaemonLog::ErrorEntry);
        return false;
    }

    QStringList arguments = parser.positionalArguments();
    QStringList plistArguments;

    if (arguments.size() > 0) {
        plistArguments << QStringLiteral("<string>--</string>");
        for (const QString& argument: arguments)
            plistArguments << QStringLiteral("        <string>") + argument + QStringLiteral("</string>");
    }

    QString plistTemplateText = QString::fromUtf8(plistTemplate.readAll());
    plistTemplateText = plistTemplateText.replace(QStringLiteral("%%APPLICATION_IDENTIFIER%%"), daemonName())
                                         .replace(QStringLiteral("%%APPLICATION_PATH%%"), qApp->applicationFilePath())
                                         .replace(QStringLiteral("%%ARGUMENTS%%"), plistArguments.join('\n'));

    plistOutFile.write(plistTemplateText.toUtf8());

    QSettings settings;
    settings.setValue(kPlistFilePathKey, daemonConfigurationFilePath);

    QMetaObject::invokeMethod(qApp, "installed", Qt::QueuedConnection);
    return true;
}

bool ControllerBackendOSX::uninstall()
{
    if (!stop()) {
        qDaemonLog(QStringLiteral("Couldn't stop the daemon"), QDaemonLog::ErrorEntry);
        return false;
    }

    QSettings settings;
    const QString daemonConfigurationFilePath = settings.value(kPlistFilePathKey, QString()).toString();

    if (!QFile::remove(daemonConfigurationFilePath)) {
        qDaemonLog(QStringLiteral("Couldn't remove configuration file. Check your permissions for %1!").arg(daemonConfigurationFilePath), QDaemonLog::ErrorEntry);
        return false;
    }

    settings.setValue(kPlistFilePathKey, QString());

    QMetaObject::invokeMethod(qApp, "uninstalled", Qt::QueuedConnection);
    return true;
}

QString ControllerBackendOSX::configurationPath() const
{
    QString daemonConfigurationFilePath = kDaemonsPath;
    if (parser.isSet(agentOption)) {
        if (parser.isSet(userOption))
            daemonConfigurationFilePath = kUserAgentsPath.arg(QDir::homePath());
        else
            daemonConfigurationFilePath = kSystemAgentsPath;
    }
    return daemonConfigurationFilePath;
}

QString ControllerBackendOSX::daemonTargetFileName() const
{
    QSettings settings;
    QFileInfo settingsFileInfo(settings.fileName());
    return settingsFileInfo.fileName();
}

QString ControllerBackendOSX::daemonName() const
{
    QString daemonFileName = daemonTargetFileName();
    return daemonFileName.left(daemonFileName.lastIndexOf('.'));
}

QString ControllerBackendOSX::configurationFilePath() const
{
    return configurationPath() + QStringLiteral("/") + daemonTargetFileName();
}

QAbstractControllerBackend::DaemonStatus ControllerBackendOSX::status()
{
    QAbstractControllerBackend::DaemonStatus status = NotRunningStatus;
    QProcess launchctl;
    launchctl.start(QStringLiteral("launchctl"), QStringList() << QStringLiteral("list"));

    bool success = launchctl.waitForStarted();

    if (!success) {
        qDaemonLog(QStringLiteral("Couldn't run launchctl %1").arg(QString::fromUtf8(launchctl.readAllStandardError())), QDaemonLog::ErrorEntry);
        return status;
    }

    success = launchctl.waitForFinished();

    if (success) {
        QRegularExpression re(QStringLiteral("(\\d+)\\t") + daemonName());
        QString launchctlOutput = QString::fromUtf8(launchctl.readAllStandardOutput());

        QRegularExpressionMatch match = re.match(launchctlOutput);
        if (match.hasMatch()) {
            int daemonStatus = match.captured(1).toInt();
            if (!daemonStatus)
                status = RunningStatus;
        }
    }
    else
        qDaemonLog(QStringLiteral("Couldn't start the daemon %1").arg(QString::fromUtf8(launchctl.readAllStandardError())), QDaemonLog::ErrorEntry);

    return status;
}

QT_END_NAMESPACE
