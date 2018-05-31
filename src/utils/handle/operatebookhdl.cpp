#include "operatebookhdl.h"
#include "globalInfo.h"
using namespace globalInfo;

operatebookhdl::operatebookhdl(const QString& token): handle(token){}

void operatebookhdl::deal(const QString &command, const QJsonObject &json)
{
    QByteArray cpath = command.toLocal8Bit();
    char* cmd = cpath.data();
    sql::basicSQL *msql = nullptr;
    QMetaEnum me = QMetaEnum::fromType<operatebookhdl::CMD>();
    switch(me.keyToValue(cmd))
    {
    case createbook:
        if(ID>0)
          {
            if(group!="staffs")
              {
                HDL_PERM_DENIED(jsonReturn)
              }
            else
              {
                QJsonArray info = json.value("info").toArray();
                QMap<QString,QVariant> map;
                map.insert(map.end(),"ID",0);
                for(const QJsonValue& x:info)
                  {
                    QJsonObject obj = x.toObject();
                    map.insert(map.end(),obj.value("field").toString(),obj.value("data").toString());
                  }
                msql= new sql::insert(dbFullPrefix + "books",map);
                if(msql->exec())
                  {
                    HDL_SUCCESS(jsonReturn)
                  }
                else
                  {
                    HDL_DB_ERROR(jsonReturn)
                  }
              }
          }
        else
          {
            HDL_INV_TOKEN(jsonReturn)
          }
        break;
    case deletebook:

        break;
    case changebook:

        break;
    case changegroup:

        break;
    }
}
