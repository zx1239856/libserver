#include "dbsettings.h"
#include "utils/dbwrapper/db-operation.h"
#include "globalInfo.h"
#include <QMap>
#include <QDateTime>

dbSettings::dbSettings()
{}

QString dbSettings::getSetting(const QString valName)
{
  using namespace globalInfo;
  QString str;
  sql::select sel(dbFullPrefix+"settings","variable='"+valName+"'",{"value"});
  if(sel.exec())
    {
      auto res = sel.toResult();
      if(!res.empty())
        {
          str = res[0].value("value").toString();
        }
      else
        {
          qDebug() << valName +" not present in db";
        }
    }
  else
    {
      qDebug() << "Failed to fetch "+valName+" from db";
    }
  return str;
}

bool dbSettings::setSetting(const QString valName, const QString value, unsigned int opID)
{
  using namespace globalInfo;
  QMap<QString,QVariant> list;
  list["value"]=value;
  list["set_by"]=QString::number(opID);
  list["set_time"]=QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss");
  sql::update up(dbFullPrefix+"settings",list,"variable='"+valName+"'");
  return up.exec();
}
