#include "socketthread.h"

socketThread::socketThread(qintptr socketDescriptor, QObject *parent):
    QThread(parent), socketDescriptor(socketDescriptor){}

socketThread::~socketThread()
{
    *lstream << "Disconnected from " + socketDescriptor;
    tcpsocket->close();
    tcpsocket->deleteLater();
    exit(0);
}

void socketThread::run()
{
    tcpsocket = new QTcpSocket();
    if (!tcpsocket->setSocketDescriptor(socketDescriptor))
    {
        *lstream << "Error: CANNOT set SocketDescriptor";
        return;
    }
    tcpsocket->setReadBufferSize(2048);

    *lstream << "New thread for the connection from " + tcpsocket->peerAddress().toString();
    connect(tcpsocket, &QAbstractSocket::disconnected, tcpsocket, &QAbstractSocket::deleteLater);

    Object obj(tcpsocket);
    connect(tcpsocket, &QAbstractSocket::readyRead, &obj, &Object::slot);

    tcpsocket->waitForDisconnected();
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
                throw("Bad Request");
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
            throw("Bad Request");
    }
    catch(QString msg)
    {
        *lstream << "Error: " + msg;
        tcpsocket->disconnect();
    }
}
