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

#ifndef QDAEMONAPPLICATION_H
#define QDAEMONAPPLICATION_H

#include "qdaemon-global.h"

#include <QtCore/qcoreapplication.h>

QT_BEGIN_NAMESPACE

class QDaemonApplicationPrivate;
class Q_DAEMON_EXPORT QDaemonApplication : public QCoreApplication
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QDaemonApplication)
    Q_DISABLE_COPY(QDaemonApplication)

    Q_PROPERTY(bool autoQuit READ autoQuit WRITE setAutoQuit)
    Q_PROPERTY(QString applicationDescription READ applicationDescription WRITE setApplicationDescription)

public:
    QDaemonApplication(int & argc, char ** argv,const QString &path);
    ~QDaemonApplication() Q_DECL_OVERRIDE;

    static int exec();
    static QDaemonApplication * instance();

    bool autoQuit() const;
    void setAutoQuit(bool);

    static QString applicationDescription();
    static QString applicationName();
    static void setApplicationDescription(const QString &);
    static void setApplicationName(const QString &);

Q_SIGNALS:
    void daemonized(const QStringList &);

    void started();
    void stopped();
    void installed();
    void uninstalled();

private:
    QDaemonApplicationPrivate * d_ptr;
};

QT_END_NAMESPACE

#endif // QDAEMONAPPLICATION_H
