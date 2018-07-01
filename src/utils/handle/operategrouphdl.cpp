#include "operategrouphdl.h"

using namespace sql;
using namespace globalInfo;

operategrouphdl::operategrouphdl(const QString& token): handle(token){}

void operategrouphdl::deal(const QString &command, const QJsonObject &json)
{
    QByteArray cpath = command.toLocal8Bit();
    char* cmd = cpath.data();

    QMetaEnum me = QMetaEnum::fromType<operategrouphdl::CMD>();

    // update token status
    ctrl->UpdateStatus(token);

    switch(me.keyToValue(cmd))
    {
    case createreader:
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
                sql::insert msql(dbFullPrefix + "readergroup", map);
                connect(&msql, &sql::insert::onFail, [=](const QSqlError &err){
                    qDebug()<<err;
                });
                if(msql.exec())
                    {
                        dbLog::log("createReaderGroup","The admin created a new reader group",ID,dbLog::staff);
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
    case createstaff:
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
                sql::insert msql(dbFullPrefix + "staffgroup", map);
                connect(&msql, &sql::insert::onFail, [=](const QSqlError &err){
                    qDebug()<<err;
                });
                if(msql.exec())
                    {
                        dbLog::log("createStaffGroup","The admin created a new staff group",ID,dbLog::staff);
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
                    if(obj.value("data").isString())
                        map.insert(map.end(),obj.value("field").toString(),obj.value("data").toString());
                    else if(obj.value("data").isBool())
                        map.insert(map.end(),obj.value("field").toString(),obj.value("data").toBool());
                    else if(obj.value("data").isDouble())
                        map.insert(map.end(),obj.value("field").toString(),obj.value("data").toDouble());
                }
                sql::insert msql(dbFullPrefix + "bookgroup", map);
                connect(&msql, &sql::insert::onFail, [=](const QSqlError &err){
                    qDebug()<<err;
                });
                if(msql.exec())
                    {
                        dbLog::log("createBookGroup","The admin created a new book group",ID,dbLog::staff);
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
    case changereader:
        if(ID>0)
        {
            if(group!="staffs")
            {
                HDL_PERM_DENIED(jsonReturn)
            }
            else
            {
                QJsonArray info = json.value("info").toArray();
                QString groupid = QString::number(json.value("groupid").toInt());
                QMap<QString,QVariant> map;
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
                sql::update msql(dbFullPrefix + "readergroup", map, "ID = " + groupid);
                connect(&msql, &sql::basicSQL::onFail, [=](const QSqlError &err){
                    qDebug()<<err;
                });
                if(msql.exec())
                    {
                        dbLog::log("changeReaderGroup","The admin changed a reader group",ID,dbLog::staff);
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
    case changestaff:
        if(ID>0)
        {
            if(group!="staffs")
            {
                HDL_PERM_DENIED(jsonReturn)
            }
            else
            {
                QJsonArray info = json.value("info").toArray();
                QString groupid = QString::number(json.value("groupid").toInt());
                QMap<QString,QVariant> map;
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
                sql::update msql(dbFullPrefix + "staffgroup", map, "ID = " + groupid);
                connect(&msql, &sql::basicSQL::onFail, [=](const QSqlError &err){
                    qDebug()<<err;
                });
                if(msql.exec())
                    {
                        dbLog::log("changeStaffGroup","The admin changed a group staff",ID,dbLog::staff);
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
    case changebook:
        if(ID>0)
        {
            if(group!="staffs")
            {
                HDL_PERM_DENIED(jsonReturn)
            }
            else
            {
                QJsonArray info = json.value("info").toArray();
                QString groupid = QString::number(json.value("groupid").toInt());
                QMap<QString,QVariant> map;
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
                sql::update msql(dbFullPrefix + "bookgroup", map, "ID = " + groupid);
                connect(&msql, &sql::basicSQL::onFail, [=](const QSqlError &err){
                    qDebug()<<err;
                });
                if(msql.exec())
                    {
                        dbLog::log("changeBookGroup","The admin changed a book group",ID,dbLog::staff);
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
    case deletereader:
        if(ID>0)
        {
            if(group!="staffs")
            {
                HDL_PERM_DENIED(jsonReturn)
            }
            else
            {
                int groupid = json.value("groupid").toInt();
                sql::select query(dbFullPrefix + "readers", "groupid = " + QString::number(groupid));
                if(query.exec())
                {
                    if(query.toResult().size() == 0)
                    {
                        sql::del msql(dbFullPrefix + "readergroup", "ID = " + QString::number(groupid));
                        if(msql.exec())
                        {
                            dbLog::log("deleteReaderGroup","The admin deleted a reader group.",ID,dbLog::staff);
                            HDL_SUCCESS(jsonReturn)
                        }
                        else
                        {
                            HDL_DB_ERROR(jsonReturn)
                            logDbErr(&msql);
                        }
                    }
                    else
                    {
                        jsonReturn.insert("result", false);
                        jsonReturn.insert("detail", "the group is not empty.");
                    }
                }
                else
                {
                    HDL_DB_ERROR(jsonReturn)
                    logDbErr(&query);
                }
            }
        }
        else
        {
            HDL_INV_TOKEN(jsonReturn)
        }
        break;
    case deletestaff:
        if(ID>0)
        {
            if(group!="staffs")
            {
                HDL_PERM_DENIED(jsonReturn)
            }
            else
            {
                int groupid = json.value("groupid").toInt();
                sql::select query(dbFullPrefix + "staffs", "groupid = " + QString::number(groupid));
                if(query.exec())
                {
                    if(query.toResult().size() == 0)
                    {
                        sql::del msql(dbFullPrefix + "staffgroup", "ID = " + QString::number(groupid));
                        if(msql.exec())
                        {
                            dbLog::log("deleteStaffGroup","The admin deleted a staff group.",ID,dbLog::staff);
                            HDL_SUCCESS(jsonReturn)
                        }
                        else
                        {
                            HDL_DB_ERROR(jsonReturn)
                            logDbErr(&msql);
                        }
                    }
                    else
                    {
                        jsonReturn.insert("result", false);
                        jsonReturn.insert("detail", "the group is not empty.");
                    }
                }
                else
                {
                    HDL_DB_ERROR(jsonReturn)
                    logDbErr(&query);
                }
            }
        }
        else
        {
            HDL_INV_TOKEN(jsonReturn)
        }
        break;
    case deletebook:
        if(ID>0)
        {
            if(group!="staffs")
            {
                HDL_PERM_DENIED(jsonReturn)
            }
            else
            {
                int groupid = json.value("groupid").toInt();
                sql::select query(dbFullPrefix + "books", "groupid = " + QString::number(groupid));
                if(query.exec())
                {
                    if(query.toResult().size() == 0)
                    {
                        sql::del msql(dbFullPrefix + "bookgroup", "ID = " + QString::number(groupid));
                        if(msql.exec())
                        {
                            dbLog::log("deleteBookGroup","The admin deleted a book group.",ID,dbLog::staff);
                            HDL_SUCCESS(jsonReturn)
                        }
                        else
                        {
                            HDL_DB_ERROR(jsonReturn)
                            logDbErr(&msql);
                        }
                    }
                    else
                    {
                        jsonReturn.insert("result", false);
                        jsonReturn.insert("detail", "the group is not empty.");
                    }
                }
                else
                {
                    HDL_DB_ERROR(jsonReturn)
                    logDbErr(&query);
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
