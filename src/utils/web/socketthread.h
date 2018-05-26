#ifndef SOCKETTHREAD_H
#define SOCKETTHREAD_H

#include <QThread>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDataStream>
#include <QMutexLocker>
#include "utils/web/requesthdl.h"

class socketThread: public QThread
{
    Q_OBJECT
public:
    socketThread(qintptr socketDescriptor, QObject *parent = 0);
    ~socketThread();
    void react();
protected:
    virtual void run();

private:
    QMutex m_mutex;
    qintptr socketDescriptor;
    QTcpSocket *tcpsocket;
};

class Object:public QObject
{
    Q_OBJECT
public:
    Object(QTcpSocket* tcpsocket);

public slots:
    void slot();

private:
    QTcpSocket* tcpsocket;
};
#endif // SOCKETTHREAD_H
