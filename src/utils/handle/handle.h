#ifndef HANDLE_H
#define HANDLE_H

#include <QObject>
#include <QJsonObject>
#include <QMetaEnum>
#include <QTcpSocket>
#include "utils/handle/controlhdl.h"
#include "utils/dbwrapper/db-operation.h"
#include "utils/crypto/token.h"
#include "utils/exception.h"
#include "globalInfo.h"
#include "qdaemonlog.h"

#define HDL_DB_ERROR(json) json.insert("result", false); \
json.insert("detail", "SQL query failed");

#define HDL_INV_TOKEN(json) json.insert("result", false); \
  json.insert("detail", "Invalid token");

#define HDL_PERM_DENIED(json) json.insert("result", false); \
  json.insert("detail", "Permission denied");

#define HDL_SUCCESS(json) json.insert("result", true);

class handle:public QObject
{
    Q_OBJECT

public:
    handle(const QString& token);
    virtual void deal(const QString &cmd, const QJsonObject &json) = 0;
    QJsonObject GetReturn();

protected:
    QTcpSocket *socket;
    QString token;
    int ID;
    QString group;
    QJsonObject jsonReturn;
};

inline void logDbErr(sql::basicSQL* query)
{
    auto err = query->lastError();
    qDaemonLog("Error code: "+err.nativeErrorCode()+","+err.driverText()+err.databaseText(),QDaemonLog::ErrorEntry);
}

#endif // HANDLE_H
