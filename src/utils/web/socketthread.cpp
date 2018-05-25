#include "socketthread.h"
#include "qdaemonlog.h"

inline QString ipParser(const QHostAddress& addr)
{
  // in newer version of Qt5
  // it automatically converts ipv4 to ipv6 and gives a weird :ffff::[addr]
  // so we need to use this trivial way to parse the address
  bool ok = false;
  QHostAddress ip4addr(addr.toIPv4Address(&ok));
  if(ok)
    {
      return ip4addr.toString();
    }
  else return addr.toString();
}

socketThread::socketThread(qintptr socketDescriptor, QObject *parent):
    QThread(parent), socketDescriptor(socketDescriptor){}

socketThread::~socketThread()
{
    tcpsocket->deleteLater();
}

void socketThread::run()
{
    tcpsocket = new QTcpSocket();
    if (!tcpsocket->setSocketDescriptor(socketDescriptor))
    {
        qDaemonLog("CANNOT set SocketDescriptor",QDaemonLog::ErrorEntry);
        return;
    }
    tcpsocket->setReadBufferSize(2048);
    QString peerIp = "null";
    peerIp = ipParser(tcpsocket->peerAddress());
    qDaemonLog("New thread for the connection from " + peerIp);
    // things to do after disconnected
    connect(tcpsocket,&QAbstractSocket::disconnected,this,[&,peerIp]()
      {
        qDaemonLog("Disconnected from " + peerIp);
        tcpsocket->close();
        this->quit();
      });
    Object obj(tcpsocket);
    connect(tcpsocket, &QAbstractSocket::readyRead, &obj, &Object::slot);
    // set max Timeout for the socket
    if(!tcpsocket->waitForDisconnected(3000))
      {
        tcpsocket->disconnectFromHost();
      }
}

Object::Object(QTcpSocket* tcpsocket): tcpsocket(tcpsocket){}
void Object::slot()
{
    try
    {
        QByteArray &&ReadData =tcpsocket->readAll();
        QDataStream in(&ReadData,QIODevice::ReadOnly);
        qint8 sign;
        in >> sign;
        if(sign == 'Q')
        {
            // read
            qint32 bytes;
            QByteArray rqtData;
            in >> bytes;
            in >> rqtData;
            if(rqtData.size() != bytes)
                throw(QString("Bad Request"));
            requesthdl hdl(rqtData);

            // send
            QByteArray &&dltData = hdl.deal();
            QByteArray SendData;
            QDataStream out(&SendData,QIODevice::WriteOnly);
            out << (qint8)'A';
            out << (qint32)dltData.size();
            out << dltData;
            tcpsocket->write(SendData);
            tcpsocket->flush();
        }
        else
            throw(QString("Bad Request"));
    }
    catch(QString msg)
    {
        qDaemonLog(msg , QDaemonLog::ErrorEntry);
    }
    qDaemonLog("Disconnected from " + ipParser(tcpsocket->peerAddress()));
    tcpsocket->disconnectFromHost();
}
