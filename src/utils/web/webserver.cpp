#include "webserver.h"
#include "qdaemonlog.h"

webServer::webServer(QObject *parent) : QTcpServer(parent),threadPool(new QThreadPool) {}

webServer::~webServer()
{
  if(threadPool)delete threadPool;
}

void webServer::init(int port, int ccurrency, uint keepAlive)
{
    //设置最大允许连接数，不设置的话默认为30
    Q_UNUSED(ccurrency)
    setMaxPendingConnections(50);
    // ExpiryTimeout = thread KeepAliveTime
    threadPool->setExpiryTimeout(keepAlive);
    if(listen(QHostAddress::Any, port))
    {
        qDaemonLog("Start listening to port " + QString::number(port));
    }
    else
    {
        qDaemonLog("CANNOT listen to port " + QString::number(port),QDaemonLog::ErrorEntry);
    }
}

void webServer::incomingConnection(qintptr socketDescriptor)
{
    qDaemonLog("Trying a new connection.");
    socketThread *skt = new socketThread(socketDescriptor);
    //connect(skt, SIGNAL(finished()), skt, SLOT(deleteLater()));
    //skt->start();
    threadPool->start(skt);
}

/*
void webServer::removeUserFormList()
{
    QTcpSocket* socket = static_cast<QTcpSocket*>(sender());

    for(auto it = m_mapClient.begin(); it != m_mapClient.end(); it++)
    {
        if(socket->peerAddress().toString() == it.key())
        {
            m_mapClient.erase(it);
            break;
        }
    }
}
*/
