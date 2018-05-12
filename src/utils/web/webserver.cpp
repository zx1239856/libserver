#include "webserver.h"

webServer::webServer(QObject *parent) : QTcpServer(parent) {}

webServer::~webServer(){}

void webServer::init(int port, int ccurrency)
{
    //设置最大允许连接数，不设置的话默认为30
    setMaxPendingConnections(ccurrency);

    if(listen(QHostAddress::Any, port))
    {
        *lstream << "Start listening to port " + QString::number(port);
    }
    else
    {
        *lstream << "Error: CANNOT listen to port " + QString::number(port);
    }
}

void webServer::incomingConnection(qintptr socketDescriptor)
{
    *lstream << "Trying a new connection.";
    socketThread *skt = new socketThread(socketDescriptor);
    connect(skt, SIGNAL(finished()), skt, SLOT(deleteLater()));
    skt->start();
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
