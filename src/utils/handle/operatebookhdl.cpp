﻿#include "operatebookhdl.h"
using namespace globalInfo;

operatebookhdl::operatebookhdl(const QString& token): handle(token){}

void operatebookhdl::deal(const QString &command, const QJsonObject &json)
{
    QByteArray cpath = command.toLocal8Bit();
    char* cmd = cpath.data();
    sql::basicSQL *msql = nullptr;
    QMetaEnum me = QMetaEnum::fromType<operatebookhdl::CMD>();

    // update token status
    ctrl->UpdateStatus(token);

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
                map.insert("operatorID", ID);
                for(const QJsonValue& x:info)
                {
                    QJsonObject obj = x.toObject();
                    if(obj.value("field").toString() == "tags")
                    {
                        QJsonArray tags = obj.value("data").toArray();
                        QString strtags = ",";
                        for(auto iter:tags)
                        {
                            strtags.append(iter.toString() + ",");
                        }
                        map.insert("tags", strtags);
                    }
                    else if(obj.value("data").isString())
                        map.insert(map.end(),obj.value("field").toString(),obj.value("data").toString());
                    else if(obj.value("data").isBool())
                        map.insert(map.end(),obj.value("field").toString(),obj.value("data").toBool());
                    else if(obj.value("data").isDouble())
                        map.insert(map.end(),obj.value("field").toString(),obj.value("data").toDouble());
                }
                msql= new sql::insert(dbFullPrefix + "books", map);
                connect(msql, &sql::insert::onFail, [=](const QSqlError &err){
                    qDebug()<<err;
                });
                if(msql->exec())
                {
                    dbLog::log("createBook","The admin added book",ID,dbLog::staff);
                    HDL_SUCCESS(jsonReturn)
                }
                else
                {
                    HDL_DB_ERROR(jsonReturn)
                    logDbErr(msql);
                }
                delete msql;
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
                QJsonArray bookid = json.value("bookid").toArray();
                for(const QJsonValue& x : bookid)
                {
                    msql= new sql::del(dbFullPrefix + "books", "ID = " + QString::number(x.toInt()));
                    if(msql->exec())
                    {
                        dbLog::log("deleteBook","The admin deleted book, ID="+json.value("bookid").toString(),ID,dbLog::staff);
                        HDL_SUCCESS(jsonReturn)
                    }
                    else
                    {
                        HDL_DB_ERROR(jsonReturn)
                        logDbErr(msql);
                    }
                    delete msql;
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
                QString bookid = QString::number(json.value("bookid").toInt());
                QMap<QString,QVariant> map;
                for(const QJsonValue& x:info)
                {
                    QJsonObject obj = x.toObject();
                    if(obj.value("field").toString() == "tags")
                    {
                        QJsonArray tags = obj.value("data").toArray();
                        QString strtags = ",";
                        for(auto iter:tags)
                        {
                            strtags.append(iter.toString() + ",");
                        }
                        map.insert("tags", strtags);
                    }
                    else if(obj.value("data").isString())
                        map.insert(map.end(),obj.value("field").toString(),obj.value("data").toString());
                    else if(obj.value("data").isBool())
                        map.insert(map.end(),obj.value("field").toString(),obj.value("data").toBool());
                    else if(obj.value("data").isDouble())
                        map.insert(map.end(),obj.value("field").toString(),obj.value("data").toDouble());
                }
                msql= new sql::update(dbFullPrefix + "books", map, "ID = " + bookid);
                connect(msql, &sql::basicSQL::onFail, [=](const QSqlError &err){
                    qDebug()<<err;
                });
                 if(msql->exec())
                    {
                        dbLog::log("changeBook","The admin changed book",ID,dbLog::staff);
                        HDL_SUCCESS(jsonReturn)
                    }
                    else
                    {
                        HDL_DB_ERROR(jsonReturn)
                        logDbErr(msql);
                    }
                    delete msql;
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
                QJsonArray books = json.value("bookid").toArray();
                int groupid = json.value("groupid").toInt();
                QMap<QString,QVariant> map;
                map.insert("groupid", groupid);
                for(const QJsonValue& x:books)
                {
                    QString bookid = QString::number(x.toInt());
                    msql= new sql::update(dbFullPrefix + "books", map, "ID = " + bookid);
                    connect(msql, &sql::insert::onFail, [=](const QSqlError &err){
                        qDebug()<<err;
                    });
                     if(msql->exec())
                    {
                        dbLog::log("changeGroup","The admin changed group of books, ID="+json.value("bookid").toString(),ID,dbLog::staff);
                        HDL_SUCCESS(jsonReturn)
                    }
                    else
                    {
                        HDL_DB_ERROR(jsonReturn)
                        logDbErr(msql);
                    }
                    delete msql;
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
