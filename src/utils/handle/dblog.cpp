#include "dblog.h"
#include "qdaemonlog.h"
#include "globalInfo.h"
#include "utils/dbwrapper/db-operation.h"
#include <QDateTime>
#include <QMap>

dbLog::dbLog()
{}

void dbLog::log(const QString type, const QString content, uint operatorID, operatorType opType)
{
  using namespace globalInfo;
  QString t;
  switch(opType)
    {
    case staff: t="staff";
    case reader: t="reader";
    default: t="other";
    }
  sql::basicSQL resetter("ALTER TABLE "+dbFullPrefix +"operation_log AUTO_INCREMENT =1");
  if(!resetter.exec())
    {
      qDaemonLog("Log to db failed.",QDaemonLog::ErrorEntry);
      qDaemonLog("Opeation: ["+ type + "] " + content + " " + operatorID + " " + t);
      return;
    }
  else
    {
      QMap<QString,QVariant> list;
      list["ID"]=0;
      list["type"]=type;
      list["content"]=content;
      list["time"]=QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss");
      list["operatorID"] = operatorID;
      list["operatorType"] = t;
      sql::insert ins(dbFullPrefix+"operation_log",list);
      if(!ins.exec())
        {
          qDaemonLog("Log to db failed.",QDaemonLog::ErrorEntry);
          qDaemonLog("Opeation: ["+ type + "] " + content + " " + operatorID + " " + t);
          return;
        }
    }
}
