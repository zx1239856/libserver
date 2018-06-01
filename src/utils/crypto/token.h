#ifndef _TOKEN_H
#define _TOKEN_H

#include <QtCore/QCryptographicHash>
#include <QString>
#include <QFile>

class token
{
public:
  // get user token
    static QString getToken(const QString &username);
  // get MD5 of things
    static QString getMD5(const QString &str);
    static QString getMD5(QFile &file);
    static QString getMD5(const QImage &img);
    static QString getMD5(const QByteArray &byteArr);
};

#endif
