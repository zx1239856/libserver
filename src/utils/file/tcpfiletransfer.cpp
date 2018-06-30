#include "tcpfiletransfer.h"
#include "utils/config.h"


tcpFileTransfer::tcpFileTransfer(QTcpSocket* tcpsocket,QObject *parent) : QObject(parent),socket(tcpsocket) {}

void tcpFileTransfer::sendHead(const QString &name,QString dir)
{
    // set file info and send filehead
    fileSize = 0;
    sendSize = 0;
    fileName = name;
    if(dir.isEmpty())
    {
        dir=config::getInstance()->dataDir();
    }
    if(!fileName.isEmpty())
    {
        QFileInfo info(dir + fileName);
        fileSize = info.size();
        // readInfo okay
        file = new QFile(dir+fileName);
        if(!file->open(QIODevice::ReadOnly))
        {
            if(file)
            {
                delete file;
                file = nullptr;
            }
            emit onFail("Failed to open the file");
            return;
        }
        // read succeed, send head
        QString head = QString("HEAD##%1##%2").arg(fileName).arg(fileSize);
        qint64 len = socket->write(head.toUtf8());
        if(len<0)
        {
            file->close();
            if(file)
            {
                delete file;
                file = nullptr;
            }
            emit onFail("Failed to send header information");
            return;
        }
    }
    else
    {
        emit onFail("File name empty");
        return;
    }
}

void tcpFileTransfer::sendData()
{
    qint64 len = 0;
    do{
        char buf[BUF_SIZE] = {0};
        len = 0;
        len = file->read(buf,BUF_SIZE);  //len为读取的字节数
        len = socket->write(buf,len);    //len为发送的字节数
        socket->flush();
        sendSize += len;
    }
    while(len > 0);
    // when the loop breaks,
    // file send is completed
    file->close();
    if(file)
    {
        delete file;
        file = nullptr;
    }
    emit onFinish();
}

// this function blocks until transfer finish
void tcpFileTransfer::receiveFile(QString dir)
{
    if(dir.isEmpty())
    {
        dir=config::getInstance()->dataDir();
    }
    while(true)
    {
        socket->waitForReadyRead();
        QByteArray buf = socket->readAll();
        if(true == isStart){
            isStart = false;
            // get head
            fileName = QString(buf).section("##",1,1);
            fileSize = QString(buf).section("##",2,2).toInt();
            recvSize = 0;
            QString str = QString("File receive:[%1:%2kB]").arg(fileName).arg(fileSize/1024);
#ifdef VERBOSE_OUTPUT
            qDebug() << str;
#endif
            if(fileName.isEmpty())
            {
                emit onFail("File name empty, maybe file head is corrupted?");
            }
            file = new QFile(dir + fileName);
            if(false == file->open(QIODevice::WriteOnly)){
                emit onFail("Failed to open the file to write");
                // error handling
            }
            socket->write("FILE##HEAD##RCV");
        }else{
            qint64 len = file->write(buf);
            recvSize += len;
            if(recvSize == fileSize){
                file->close();
                socket->write("FILE##FINISH");
                socket->disconnectFromHost();
                socket->close();
                if(file)
                {
                    delete file;
                    file = nullptr;
                }
                emit onFinish();
                break;
            }
        }
    }
}


