#ifndef _TOKEN_H
#define _TOKEN_H

#include <QtCore/QCryptographicHash>
#include <QString>

class token
{
public:
    static QString getToken(const QString &username);
};

#endif