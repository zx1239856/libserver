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

#include "qdaemonapplication.h"
#include "private/qdaemonapplication_p.h"
#include "private/qabstractdaemonbackend.h"

#include <QtCore/QScopedPointer>

QT_BEGIN_NAMESPACE

using namespace QtDaemon;
/*!
    \class QDaemonApplication
    \inmodule QtDaemon

    \brief The \l{QDaemonApplication} class provides an event loop for
    system-level services (deamons).

    It processes the application command line and provides for
    both the daemon itself and the controlling application.
    The application behavior is controlled through command line switches
    passed on startup. When no switches are provided the command line help text
    is displayed. The application object will emit the appropriate signal
    associated with a switch when there are no errors processing the command line.

    \include qtdaemon-cl-switches.qdocinc

    \sa QCoreApplication
*/
/*!
    \fn void QDaemonApplication::daemonized(const QStringList & arguments)

    This signal is emitted when the process has been daemonized.
    The string list argument \a arguments is a proper command line that
    can be used with a \l{QCommandLineParser} instance.
*/
/*!
    \fn void QDaemonApplication::started()

    This signal is emitted when the application is run as a control terminal
    and the daemon process is started successfully.
*/
/*!
    \fn void QDaemonApplication::stopped()

    This signal is emitted when the application is run as a control terminal
    and the daemon process is stopped successfully.
*/
/*!
    \fn void QDaemonApplication::installed()

    This signal is emitted when the application is run as a control terminal
    and the daemon application has been installed successfully.
*/
/*!
    \fn void QDaemonApplication::uninstalled()

    This signal is emitted when the application is run as a control terminal
    and the daemon application has been uninstalled successfully.
*/

/*!
    Constructs the daemon application object.

    The \a argc and \a argv arguments are processed by the application and are
    made available through the \l{QDaemonApplication::}{daemonized()} signal.

    \warning The data referred to by \a argc and \a argv must stay valid
    for the entire lifetime of the application object.

    \warning Do not rely on the arguments() function as there are command-line
    parameters used internally by the class.
*/
QDaemonApplication::QDaemonApplication(int & argc, char ** argv,const QString &logPath)
    : QCoreApplication(argc, argv), d_ptr(new QDaemonApplicationPrivate(this,logPath))
{
}

/*!
    Destroys the QDaemonApplicaiton object.
*/
QDaemonApplication::~QDaemonApplication()
{
    delete d_ptr;
}

/*!
    Starts the application event loop and executes the daemon application.

    \sa quit(), exit(), processEvents(), QCoreApplication::exec()
*/
int QDaemonApplication::exec()
{
    // Just check if someone forgot to create the application object
    QDaemonApplication * app = QDaemonApplication::instance();
    if (Q_UNLIKELY(!app))  {
        qWarning("You must construct a QDaemonApplication before calling QDaemonApplication::exec.");
        return -1;
    }

    QStringList arguments = QDaemonApplication::arguments();
    QDaemonApplicationPrivate * const d = app->d_ptr;

    QCommandLineOption daemonOption(QStringLiteral("d"));
    //daemonOption.setHidden(true);
    daemonOption.setFlags(QCommandLineOption::HiddenFromHelp);
    d->parser.addOption(daemonOption);
    d->parser.parse(arguments);

    // Create the appropriate backend
    bool isDaemon = d->parser.isSet(daemonOption);
    QScopedPointer<QAbstractDaemonBackend> backend(d->createBackend(isDaemon));

    // Reparse with the options that the backends may have added in their constructors
    d->parser.parse(arguments);

    return backend->exec();
}

/*!
    Returns a pointer to the application's QDaemonApplication instance.

    If no instance has been created, \c null is returned.
*/
QDaemonApplication * QDaemonApplication::instance()
{
    return qobject_cast<QDaemonApplication *>(QCoreApplication::instance());
}

/*!
    \property QDaemonApplication::autoQuit
    \brief Holds whether the daemon application should automatically exit the event loop.

    If the property is set to \c true and the application process is running as control terminal
    the application will quit automatically after emitting the proper signal or reporting errors.

    In case you need to attach some code that should be run after the daemon has started/stopped
    or after it has been installed/uninstalled you can disable the controlling process' auto
    quit feature and subscribe to the appropriate signal.

    By default this property is \c true.

    \note The property is enforced to \c false when the application is run with \c --fake.
    \note The property is enforced to \c true when the command line help is requested.
*/
bool QDaemonApplication::autoQuit() const
{
    Q_D(const QDaemonApplication);
    return d->autoQuit;
}

void QDaemonApplication::setAutoQuit(bool enable)
{
    Q_D(QDaemonApplication);
    d->autoQuit = enable;
}

/*!
    \property QDaemonApplication::applicationDescription
    \brief Holds the daemon application's description.

    \note The description is not required, but if supplied it will be used to provide extra
    information with the service control manager (windows) or the init.d script (linux).
*/
QString QDaemonApplication::applicationDescription()
{
    return QDaemonApplicationPrivate::description;
}

QString QDaemonApplication::applicationName()
{
    return QDaemonApplicationPrivate::name;
}

void QDaemonApplication::setApplicationDescription(const QString & description)
{
    QDaemonApplicationPrivate::description = description;
}

void QDaemonApplication::setApplicationName(const QString & _name)
{
    QDaemonApplicationPrivate::name = _name;
    QCoreApplication::setApplicationName(_name);
}

QT_END_NAMESPACE
