#ifndef REQUESTHDL_H
#define REQUESTHDL_H

#include <QJsonObject>
#include <QJsonDocument>
#include "utils/file/logstream.h"

class requesthdl
{
    QJsonObject jsonRequest;
    QJsonObject jsonReturn;
public:
    requesthdl(const QByteArray &rqtData);
    QByteArray deal();
};

#endif // REQUESTHDL_H
