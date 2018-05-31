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
  QTcpSocket *socket;
  QString fileName;
  qint64 fileSize=0;
  qint64 sendSize=0;
  QFile *file=nullptr;
public:
  explicit tcpFileTransfer(QTcpSocket* tcpsocket,QObject *parent = nullptr);
  void sendHead(const QString &fileName,QString dir="");
  void sendData();
signals:
  void onFail(const QString &err);
  void onFinish();
};

#endif // TCPFILETRANSFER_H
