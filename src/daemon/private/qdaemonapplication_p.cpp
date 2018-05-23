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

#include "qdaemonapplication_p.h"
#include "qdaemonapplication.h"
#include "qdaemonlog_p.h"

#include <csignal>

#if defined(Q_OS_WIN)
#include "daemonbackend_win.h"
#include "controllerbackend_win.h"
#elif defined(Q_OS_LINUX)
#include "daemonbackend_linux.h"
#include "controllerbackend_linux.h"
#elif defined(Q_OS_OSX)
#include "daemonbackend_osx.h"
#include "controllerbackend_osx.h"
#else
#warning This library is not supported on your platform.
#endif

QT_BEGIN_NAMESPACE

using namespace QtDaemon;

#if defined(Q_OS_WIN)
typedef DaemonBackendWindows DaemonBackend;
typedef ControllerBackendWindows ControllerBackend;
#elif defined(Q_OS_LINUX)
typedef DaemonBackendLinux DaemonBackend;
typedef ControllerBackendLinux ControllerBackend;
#elif defined(Q_OS_OSX)
typedef DaemonBackendOSX DaemonBackend;
typedef ControllerBackendOSX ControllerBackend;
#endif

QString QDaemonApplicationPrivate::description;

QDaemonApplicationPrivate::QDaemonApplicationPrivate(QDaemonApplication * q)
    : q_ptr(q), log(*new QDaemonLogPrivate), autoQuit(true)
{
    std::signal(SIGTERM, QDaemonApplicationPrivate::processSignalHandler);
    std::signal(SIGINT, QDaemonApplicationPrivate::processSignalHandler);
    std::signal(SIGSEGV, QDaemonApplicationPrivate::processSignalHandler);
}

QDaemonApplicationPrivate::~QDaemonApplicationPrivate()
{
}

void QDaemonApplicationPrivate::processSignalHandler(int signalNumber)
{
    switch (signalNumber)
    {
    case SIGSEGV:
       ::exit(-1);
    case SIGTERM:
    case SIGINT:
        {
            QDaemonApplication * app = QDaemonApplication::instance();
            if (app)
                app->quit();
            else
                ::exit(-1);
        }
        break;
    default:
        return;
    }
}

QAbstractDaemonBackend * QDaemonApplicationPrivate::createBackend(bool isDaemon)
{
    if (isDaemon)  {
        log.setLogType(QDaemonLog::LogToFile);
        return new DaemonBackend(parser);
    }
    else
        return new ControllerBackend(parser, autoQuit);
}

QT_END_NAMESPACE
