#ifndef HANDLE_H
#define HANDLE_H

#include <QObject>
#include <QJsonObject>
#include <QMetaEnum>
#include "utils/handle/controlhdl.h"
#include "utils/dbwrapper/db-operation.h"
#include "utils/cryto/token.h"
#include "utils/exception.h"

#define HDL_DB_ERROR(json) json.insert("result", false); \
json.insert("detail", "database server error");

#define HDL_INV_TOKEN(json) json.insert("result", false); \
  json.insert("detail", "invalid token");

#define HDL_PERM_DENIED(json) json.insert("result", false); \
  json.insert("detail", "permission denied");

#define HDL_SUCCESS(json) json.insert("result", true);

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
