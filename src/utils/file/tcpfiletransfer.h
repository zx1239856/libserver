#ifndef TCPFILETRANSFER_H
#define TCPFILETRANSFER_H

#include <QObject>
#include <QTcpSocket>
#include <QFile>

const uint BUF_SIZE=1024*4;

class tcpFileTransfer : public QObject
{
  Q_OBJECT
private:
  // common part
  QTcpSocket *socket;
  QString fileName;
  qint64 fileSize=0;
  QFile *file=nullptr;
  // send file
  qint64 sendSize=0;
  // receive file
  qint64 recvSize=0;
  bool isStart = true;
public:
  explicit tcpFileTransfer(QTcpSocket* tcpsocket,QObject *parent = nullptr);
  void sendHead(const QString &fileName,QString dir="");
  void sendData();
  // receive file
  void receiveFile(QString dir="");
signals:
  void onFail(const QString &err);
  void onFinish();
};

#endif // TCPFILETRANSFER_H
