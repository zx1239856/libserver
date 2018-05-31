#include "tcpfiletransfer.h"
#include "utils/config.h"


tcpFileTransfer::tcpFileTransfer(QTcpSocket* tcpsocket,QObject *parent) : QObject(parent),socket(tcpsocket)
{

}

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


