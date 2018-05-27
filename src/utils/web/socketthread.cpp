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

socketThread::socketThread(qintptr socketDescriptor):
    socketDescriptor(socketDescriptor){}

socketThread::~socketThread()
{
    tcpsocket->deleteLater();
}

void socketThread::run()
{
    QMutexLocker locker(&m_mutex);
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
    QObject::connect(tcpsocket,&QAbstractSocket::disconnected,[&,peerIp]()
      {
        qDaemonLog("Disconnected from " + peerIp);
        tcpsocket->close();
        //this->quit();
      });
    Object obj(tcpsocket);
    QObject::connect(tcpsocket, &QAbstractSocket::readyRead, &obj, &Object::slot);
    // set max Timeout for the socket
    if(!tcpsocket->waitForDisconnected(300000))
      {
        tcpsocket->disconnectFromHost();
      }
}

Object::Object(QTcpSocket* tcpsocket): tcpsocket(tcpsocket){}
void Object::slot()
{
    try
    {
        QByteArray &&ReadData = tcpsocket->readAll();
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
    }
    catch(QString msg)
    {
        qDaemonLog(msg , QDaemonLog::ErrorEntry);
    }
    tcpsocket->disconnectFromHost();
}
