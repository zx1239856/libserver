#ifndef HANDLE_H
#define HANDLE_H

#include <QObject>
#include <QJsonObject>
#include <QMetaEnum>
#include "utils/handle/controlhdl.h"
#include "utils/dbwrapper/db-operation.h"
#include "utils/cryto/token.h"
#include "utils/exception.h"

class handle:public QObject
{
    Q_OBJECT

public:
    handle(const QString& token);
    virtual void deal(const QString &cmd, const QJsonObject &json) = 0;
    QJsonObject GetReturn();

protected:
    QString token;
    int ID;
    QString group;
    QJsonObject jsonReturn;
};

#endif // HANDLE_H
