#include "token.h"
#include <QByteArray>
#include <QDateTime>

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
