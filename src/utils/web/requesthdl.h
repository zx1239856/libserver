#ifndef REQUESTHDL_H
#define REQUESTHDL_H

#include <QJsonObject>
#include <QJsonDocument>
#include <QTcpSocket>
#include <QObject>

class requesthdl: public QObject
{
    Q_OBJECT

    QJsonObject jsonRequest;
public:
    requesthdl(const QByteArray &rqtData);
    QByteArray deal();
    QString mode;
    QString file;
};

#endif // REQUESTHDL_H
