#include "socketthread.h"

socketThread::socketThread(qintptr socketDescriptor, QObject *parent):
    QThread(parent), socketDescriptor(socketDescriptor){}

socketThread::~socketThread()
{
    *lstream << "Disconnected from " + tcpsocket->peerAddress().toString();
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
    //connect(tcpsocket, &QAbstractSocket::disconnected, this, &socketThread::disconnect);

    connect(tcpsocket, &QAbstractSocket::readyRead, this, &socketThread::React);

    tcpsocket->write("Welcome to libserver!");
    tcpsocket->flush();

    tcpsocket->waitForDisconnected();
}

void socketThread::React()
{
    /*
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
            requesthdl* hdl;
            if(ReadData.size() == bytes)
                hdl = new requesthdl(ReadData);
            else
                throw("Bad Request");

            // send
            SendData = hdl->deal();
            delete hdl;
            tcpsocket->write(SendData);
            tcpsocket->flush();
        }
        else
            throw("Bad Request");
    }
    catch(QString msg)
    {
        *lstream << "Error: " + msg;
        tcpsocket->close();
    }
    */
}
