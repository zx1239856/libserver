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
    qDaemonLog("New incoming connection from " + peerIp);
    // things to do after disconnected
    QObject::connect(tcpsocket,&QTcpSocket::disconnected,[&,peerIp]()
      {
        qDaemonLog("Disconnected from " + peerIp);
        tcpsocket->close();
        //this->quit();
      });
    Object obj(tcpsocket);
    QObject::connect(tcpsocket, &QTcpSocket::readyRead, &obj, &Object::slot);
    // set max Timeout for the socket
    if(!tcpsocket->waitForDisconnected(100000))
      {
        tcpsocket->disconnectFromHost();
      }
}

Object::Object(QTcpSocket* tcpsocket): tcpsocket(tcpsocket),fileTransfer(nullptr){}

Object::~Object()
{
  if(fileTransfer)delete fileTransfer;
}

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
            {
                // return bad request
                throw(QString("Bad Request"));
            }
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
        if(sign == 'F') // if it is file transfer
          {
            qint32 page;
            in >> page;
            fileTransfer=new tcpFileTransfer(tcpsocket);
            // change signal receiver here
            tcpsocket->disconnect(tcpsocket,&QTcpSocket::readyRead,this,&Object::slot);
            QObject::connect(tcpsocket,&QTcpSocket::readyRead,fileTransfer,[&]()
            {
                QByteArray buf = tcpsocket->readAll();
                if("FILE##HEAD##RCV" == QString(buf))
                {
                    // file header recv succeed
                    fileTransfer->sendData();
                }
                else if("FILE##FINISH" == QString(buf))
                  {
                    // file transfer complete
                    tcpsocket->disconnectFromHost();
                    tcpsocket->close();
                  }
            });
            QObject::connect(fileTransfer,&tcpFileTransfer::onFail,this,[&](const QString &err)
            {
                qDebug() << err;
                // echo err msg to client
            });
            fileTransfer->sendHead("output/file_" + QString::number(page)+".jpg");
            QObject::connect(fileTransfer,&tcpFileTransfer::onFinish,this,[&]()
            {
                fileTransfer->deleteLater();
                fileTransfer = nullptr;
            });
          }
    }
    catch(QString msg)
    {
        qDaemonLog(msg , QDaemonLog::ErrorEntry);
    }
  // if no need to transfer file, just disconnect
    if(fileTransfer==nullptr)
        tcpsocket->disconnectFromHost();
}
