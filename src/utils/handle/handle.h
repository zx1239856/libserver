#ifndef HANDLE_H
#define HANDLE_H

#include <QObject>
#include <QJsonObject>
#include <QMetaEnum>
#include "utils/handle/controlhdl.h"
#include "utils/exception.h"

class handle:public QObject
{
    Q_OBJECT

public:
    handle(double token);
    virtual bool deal(const QString &cmd, const QJsonObject &json) = 0;
    QJsonObject GetReturn();

protected:
    qint32 ID;
    QJsonObject jsonReturn;
};

#endif // HANDLE_H