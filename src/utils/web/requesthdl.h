#ifndef REQUESTHDL_H
#define REQUESTHDL_H

#include <QJsonObject>
#include <QJsonDocument>

class requesthdl
{
    QJsonObject jsonRequest;
public:
    requesthdl(const QByteArray &rqtData);
    virtual QByteArray deal();
};

#endif // REQUESTHDL_H
