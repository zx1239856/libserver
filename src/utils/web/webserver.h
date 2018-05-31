#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QThreadPool>
#include "utils/web/socketthread.h"

class webServer: public QTcpServer
{
    Q_OBJECT
public:
    explicit webServer(QObject *parent = 0);
    void init(int port, int ccurrency);
    ~webServer();

protected:
    virtual void incomingConnection(qintptr socketDescriptor);
    //void removeUserFormList();

private:
    QMap<QString, QTcpSocket *> m_mapClient;
    QThreadPool *threadPool;
};

#endif // WEBSERVER_H
