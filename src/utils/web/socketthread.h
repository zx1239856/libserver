#ifndef SOCKETTHREAD_H
#define SOCKETTHREAD_H

#include <QThread>
#include <QTcpServer>
#include <QTcpSocket>
#include "utils/file/logstream.h"
#include "utils/web/requesthdl.h"

class socketThread: public QThread
{
    Q_OBJECT
public:
    socketThread(qintptr socketDescriptor, QObject *parent = 0);
    ~socketThread();

protected:
    virtual void run();

private slots:
    void React();

private:
    qintptr socketDescriptor;
    QTcpSocket *tcpsocket;
};

#endif // SOCKETTHREAD_H
