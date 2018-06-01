#include "token.h"
#include <QByteArray>
#include <QDateTime>
#include <QImage>
#include <QBuffer>

QString token::getToken(const QString &username)
{
  QByteArray ba;
  qsrand(qrand());
  ba.append(username);
  ba = QCryptographicHash::hash(ba,QCryptographicHash::Md5);
  QByteArray ba2;
  ba2.append(ba);
  ba2.append(QString::number(QDateTime::currentMSecsSinceEpoch()));
  ba2.append(QString::number(qrand()));
  ba2 = QCryptographicHash::hash(ba2,QCryptographicHash::Md5);
  return ba2.toHex();
}

QString token::getMD5(const QString &str)
{
  QByteArray ba;
  ba.append(str);
  return QCryptographicHash::hash(ba,QCryptographicHash::Md5).toHex();
}

QString token::getMD5(QFile &file)
{
  QCryptographicHash ch(QCryptographicHash::Md5);
  quint64 totalBytes = 0;
  quint64 bytesWritten = 0;
  quint64 bytesToWrite = 0;
  quint64 loadSize = 1024 * 4; // adjust this properly
  QByteArray buf;

  totalBytes = file.size();
  bytesToWrite = totalBytes;
  while(true)
    {
      if(bytesToWrite > 0)
        {
          buf = file.read(qMin(bytesToWrite,loadSize));
          ch.addData(buf);
          bytesWritten += buf.length();
          bytesToWrite -= buf.length();
          buf.clear();
        }
      else break;
      if(bytesWritten == totalBytes)break;
    }
  return ch.result().toHex();
}

QString token::getMD5(const QImage &img)
{
  QBuffer buff;
  img.save(&buff,"JPG");
  return token::getMD5(buff.data());
}

QString token::getMD5(const QByteArray &byteArr)
{
  return QCryptographicHash::hash(byteArr,QCryptographicHash::Md5).toHex();
}
