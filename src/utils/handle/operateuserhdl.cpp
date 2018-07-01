#include "operateuserhdl.h"
using namespace sql;
using namespace globalInfo;

operateuserhdl::operateuserhdl(const QString& token): handle(token){}

void operateuserhdl::deal(const QString &command, const QJsonObject &json)
{
    QByteArray cpath = command.toLocal8Bit();
    char* cmd = cpath.data();

    // update token status
    ctrl->UpdateStatus(token);

    QMetaEnum me = QMetaEnum::fromType<operateuserhdl::CMD>();
    switch(me.keyToValue(cmd))
    {
    case createuser:
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
                    if(obj.value("data").isString())
                        map.insert(map.end(),obj.value("field").toString(),obj.value("data").toString());
                    else if(obj.value("data").isBool())
                        map.insert(map.end(),obj.value("field").toString(),obj.value("data").toBool());
                    else if(obj.value("data").isDouble())
                        map.insert(map.end(),obj.value("field").toString(),obj.value("data").toDouble());
                }
                sql::insert msql(dbFullPrefix + "readers", map);
                connect(&msql, &sql::insert::onFail, [=](const QSqlError &err){
                    qDebug()<<err;
                });
                if(msql.exec())
                {
                    dbLog::log("createUser","The admin created a new reader",ID,dbLog::staff);
                    HDL_SUCCESS(jsonReturn)
                }
                else
                {
                    HDL_DB_ERROR(jsonReturn)
                    logDbErr(&msql);
                }
            }
        }
        else
        {
            HDL_INV_TOKEN(jsonReturn)
        }
        break;
    case deleteuser:
        if(ID>0)
        {
            if(group!="staffs")
            {
                HDL_PERM_DENIED(jsonReturn)
            }
            else
            {
                auto userid = json.value("userid").toArray();
                bool success = true;
                for(auto x:userid)
                {
                    sql::del msql(dbFullPrefix + "readers", "ID = " + QString::number(x.toInt()));
                    if(!msql.exec())
                    {
                        success = false;
                        logDbErr(&msql);
                    }
                }
                if(success)
                {
                    dbLog::log("deleteUser","The admin deleted a reader",ID,dbLog::staff);
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
    case changegroup:
        if(ID>0)
        {
            if(group!="staffs")
            {
                HDL_PERM_DENIED(jsonReturn)
            }
            else
            {
                auto userid = json.value("userid").toArray();
                int groupid = json.value("groupid").toInt();
                QMap<QString, QVariant> map;
                map.insert("groupid", groupid);
                bool success = true;
                for(auto x:userid)
                {
                    sql::update msql(dbFullPrefix + "readers", map, "ID = " + QString::number(x.toInt()));
                    if(!msql.exec())
                    {
                        success = false;
                        logDbErr(&msql);
                    }
                }
                if(success)
                {
                    dbLog::log("changeGroup","The admin changed group of user, ID="+json.value("userid").toString(),ID,dbLog::staff);
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
    }
}
