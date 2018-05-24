#ifndef REQUESTHDL_H
#define REQUESTHDL_H

#include <QJsonObject>
#include <QJsonDocument>
#include "utils/handle/userhdl.h"
#include "utils/exception.h"

class requesthdl
{
    QJsonObject jsonRequest;
public:
    requesthdl(const QByteArray &rqtData);
    virtual QByteArray deal();
};

#endif // REQUESTHDL_H
