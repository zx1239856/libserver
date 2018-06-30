#ifndef SOCKETTHREAD_H
#define SOCKETTHREAD_H

#include <QThread>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDataStream>
#include <QMutexLocker>
#include <QRunnable>
#include "utils/web/requesthdl.h"
#include "utils/file/tcpfiletransfer.h"
#include "utils/config.h"

class socketThread: public QRunnable
{
    //Q_OBJECT
public:
    socketThread(qintptr socketDescriptor);
    ~socketThread();
    void react();
protected:
    virtual void run();

private:
    QMutex m_mutex;
    qintptr socketDescriptor;
    QTcpSocket *tcpsocket;
};


class socketThreadHandler:public QObject
{
    Q_OBJECT
public:
    socketThreadHandler(QTcpSocket* tcpsocket);
    ~socketThreadHandler();
public slots:
    void slotFunc();
private:
    QTcpSocket* tcpsocket;
    tcpFileTransfer *fileTransfer;

};
#endif // SOCKETTHREAD_H
