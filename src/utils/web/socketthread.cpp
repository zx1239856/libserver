#include "socketthread.h"

socketThread::socketThread(qintptr socketDescriptor, QObject *parent):
    QThread(parent), socketDescriptor(socketDescriptor){}

socketThread::~socketThread()
{
    *lstream << "Disconnected from " + socketDescriptor;
    tcpsocket->close();
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
        QByteArray ReadData, SendData;
        ReadData = tcpsocket->readAll();
        if(ReadData.at(0) == 'Q')
        {
            // read
            ReadData.remove(0, 1);
            int bytes = ReadData.left(4).toInt();

            ReadData.remove(0, 4);
            if(ReadData.size() != bytes)
                throw("Bad Request");
            requesthdl hdl(ReadData);

            // send
            SendData = hdl.deal();
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
