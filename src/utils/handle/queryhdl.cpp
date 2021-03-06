﻿#include "queryhdl.h"

using namespace globalInfo;

queryhdl::queryhdl(const QString& token): handle(token){}

void queryhdl::deal(const QString &command, const QJsonObject &json)
{
    QByteArray cpath = command.toLocal8Bit();
    char* cmd = cpath.data();

    QMetaEnum me = QMetaEnum::fromType<queryhdl::CMD>();
    // update token status
    ctrl->UpdateStatus(token);

    switch(me.keyToValue(cmd))
    {
    case bareSQL:
        {
          QString bare = json.value("bareSQL").toString();
          bare.replace("$dbPrefix$",dbFullPrefix);
          qDebug() << bare;
          sql::basicSQL bsql(bare);
          if(bsql.exec())
            {
              HDL_SUCCESS(jsonReturn)
              QJsonArray info;
              for(const QSqlRecord &record:bsql.toResult())
              {
                  QVariantMap info0;
                  for(int i = 0; i < record.count(); i++)
                  {
                       info0.insert(record.fieldName(i), record.value(i));
                  }
                  info.push_back(QJsonValue(QJsonObject::fromVariantMap(info0)));
              }
              jsonReturn.insert("info", QJsonValue(info));
            }
          else
            {
              HDL_DB_ERROR(jsonReturn)
              logDbErr(&bsql);
            }
        break;
        }
    case book:
        if(json.value("rule").toString() == "completesearch")
        {
            QJsonArray cond = json.value("info").toArray();
            QString conditions;
            for(auto y : cond)
            {
                auto x = y.toObject();
                if(x.value("field").toString() == "tags" && !x.value("data").toArray().isEmpty())
                {
                    QJsonArray tags = x.value("data").toArray();
                    for(auto iter:tags)
                    {
                        conditions += ("tags LIKE '%," +iter.toString() + ",%' AND ");
                    }
                }
                else if(x.value("field").toString() == "bookcase" && !x.value("data").toArray().isEmpty())
                {
                    auto z = x.value("data").toArray();
                    conditions += "(";
                    for(auto iter:z)
                    {
                        conditions += "bookcase = " + QString::number(iter.toDouble()) + " OR ";
                    }
                    conditions.chop(4);
                    conditions += ") AND ";
                }
                else if(x.value("field").toString() == "price")
                {
                    QJsonObject price = x.value("data").toObject();
                    conditions += "price<=" + QString::number(price.value("sup").toDouble()) + " AND price>=" + QString::number(price.value("inf").toDouble()) + " AND ";
                }
                else if(x.value("data").isString() && !x.value("data").toString().isEmpty())
                    conditions += x.value("field").toString() + " = '" + x.value("data").toString() + "' AND ";
                else if(x.value("data").isDouble())
                    conditions += x.value("field").toString() + " = " + QString::number(x.value("data").toDouble()) + " AND ";
                else if(x.value("data").isBool())
                    conditions += x.value("field").toString() + " = " + QString::number(x.value("data").toBool()) + " AND ";
            }
            if(!conditions.isEmpty())
                conditions.chop(5);
            else
            {
                conditions = "1";
            }

            //添加排序
            if(!json.value("order").toArray().isEmpty())
            {
                conditions += " ORDER BY ";
                for(const QJsonValue &x:json.value("order").toArray())
                {
                    conditions += x.toString() + ",";
                }
                conditions.chop(1);
            }

            //添加分页
            conditions += " LIMIT " + QString::number(json.value("records").toInt() * (json.value("page").toInt() - 1)) + "," + QString::number(json.value("records").toInt());

            qDebug() << conditions;
            sql::select msql(dbFullPrefix + "books", conditions);
            if(msql.exec())
            {
                HDL_SUCCESS(jsonReturn)
                        QJsonArray info;
                for(const QSqlRecord &record:msql.toResult())
                {
                    QVariantMap info0;
                    for(int i = 0; i < record.count(); i++)
                    {
                        if(record.fieldName(i) == "tags")
                        {
                            QStringList tagslist = record.value(i).toString().split(',', QString::SkipEmptyParts);
                            info0.insert("tags", tagslist);
                        }
                        else
                        {
                            info0.insert(record.fieldName(i), record.value(i));
                        }
                    }
                    info.push_back(QJsonValue(QJsonObject::fromVariantMap(info0)));
                }
                jsonReturn.insert("info", QJsonValue(info));
            }
            else
            {
                HDL_DB_ERROR(jsonReturn)
                        logDbErr(&msql);
            }
        }
        else if(json.value("rule").toString() == "fuzzysearch")
        {
            QJsonArray cond = json.value("info").toArray();
            QString conditions;
            for(auto y : cond)
            {
                auto x = y.toObject();
                if(x.value("field").toString() == "tags" && !x.value("data").toArray().isEmpty())
                {
                    QJsonArray tags = x.value("data").toArray();
                    for(auto iter:tags)
                    {
                        conditions += ("tags LIKE '%" +iter.toString() + "%' AND ");
                    }
                }
                else if(x.value("field").toString() == "bookcase" && !x.value("data").toArray().isEmpty())
                {
                    auto z = x.value("data").toArray();
                    conditions += "(";
                    for(auto iter:z)
                    {
                        conditions += "bookcase = " + QString::number(iter.toDouble()) + " OR ";
                    }
                    conditions.chop(4);
                    conditions += ") AND ";
                }
                else if(x.value("field").toString() == "price")
                {
                    QJsonObject price = x.value("data").toObject();
                    conditions += "price<=" + QString::number(price.value("sup").toDouble()) + " AND price>=" + QString::number(price.value("inf").toDouble()) + " AND ";
                }
                else if(x.value("data").isString() && !x.value("data").toString().isEmpty())
                    conditions += x.value("field").toString() + " LIKE '%" + x.value("data").toString() + "%' AND ";
                else if(x.value("data").isDouble())
                    conditions += x.value("field").toString() + " = " + QString::number(x.value("data").toDouble()) + " AND ";
                else if(x.value("data").isBool())
                    conditions += x.value("field").toString() + " = " + QString::number(x.value("data").toBool()) + " AND ";
            }
            if(!conditions.isEmpty())
                conditions.chop(5);
            else
            {
                conditions = "1";
            }

            //添加排序
            if(!json.value("order").toArray().isEmpty())
            {
                conditions += " ORDER BY ";
                for(const QJsonValue &x:json.value("order").toArray())
                {
                    conditions += x.toString() + ",";
                }
                conditions.chop(1);
            }

            //添加分页
            conditions += " LIMIT " + QString::number(json.value("records").toInt() * (json.value("page").toInt() - 1)) + "," + QString::number(json.value("records").toInt());

            sql::select msql(dbFullPrefix + "books", conditions);
            if(msql.exec())
            {
                HDL_SUCCESS(jsonReturn)
                        QJsonArray info;
                for(const QSqlRecord &record:msql.toResult())
                {
                    QVariantMap info0;
                    for(int i = 0; i < record.count(); i++)
                    {
                        if(record.fieldName(i) == "tags")
                        {
                            QStringList tagslist = record.value(i).toString().split(',', QString::SkipEmptyParts);
                            info0.insert("tags", tagslist);
                        }
                        else
                        {
                            info0.insert(record.fieldName(i), record.value(i));
                        }
                    }
                    info.push_back(QJsonValue(QJsonObject::fromVariantMap(info0)));
                }
                jsonReturn.insert("info", QJsonValue(info));
            }
            else
            {
                HDL_DB_ERROR(jsonReturn)
                        logDbErr(&msql);
            }
        }
        else if(json.value("rule").toString() == "intelligentsearch")
        {
            QString conditions;
            if(!json.value("info").toString().isEmpty())
            {
                QString info = json.value("info").toString();
                conditions += "name LIKE '%" + info +"%' OR author LIKE '%" + info + "%' OR press LIKE '%" + info + "%' OR tags LIKE '%" + info + "%'";
            }
            else
            {
                conditions = "1";
            }

            //添加排序
            if(!json.value("order").toArray().isEmpty())
            {
                conditions += " ORDER BY ";
                for(const QJsonValue &x:json.value("order").toArray())
                {
                    conditions += x.toString() + ",";
                }
                conditions.chop(1);
            }

            //添加分页
            conditions += " LIMIT " + QString::number(json.value("records").toInt() * (json.value("page").toInt() - 1)) + "," + QString::number(json.value("records").toInt());

            sql::select msql(dbFullPrefix + "books", conditions);
            if(msql.exec())
            {
                HDL_SUCCESS(jsonReturn)
                        QJsonArray info;
                for(const QSqlRecord &record:msql.toResult())
                {
                    QVariantMap info0;
                    for(int i = 0; i < record.count(); i++)
                    {
                        if(record.fieldName(i) == "tags")
                        {
                            QStringList tagslist = record.value(i).toString().split(',', QString::SkipEmptyParts);
                            info0.insert("tags", tagslist);
                        }
                        else
                        {
                            info0.insert(record.fieldName(i), record.value(i));
                        }
                    }
                    info.push_back(QJsonValue(QJsonObject::fromVariantMap(info0)));
                }
                jsonReturn.insert("info", QJsonValue(info));
            }
            else
            {
                HDL_DB_ERROR(jsonReturn)
                        logDbErr(&msql);
            }
        }
        break;
    case info:
        QString conditions = json.value("conditions").toString();
        if(conditions.isEmpty())
        {
            conditions = "1";
        }
        QString table = json.value("table").toString();

        //添加排序
        if(!json.value("order").toArray().isEmpty())
        {
            conditions += " ORDER BY ";
            for(const QJsonValue &x:json.value("order").toArray())
            {
                conditions += x.toString() + ",";
            }
            conditions.chop(1);
        }

        //添加分页
        conditions += " LIMIT " + QString::number(json.value("records").toInt() * (json.value("page").toInt() - 1)) + "," + QString::number(json.value("records").toInt());

        sql::select msql(dbFullPrefix + table, conditions);
        if(msql.exec())
        {
            HDL_SUCCESS(jsonReturn)
                    QJsonArray info;
            for(const QSqlRecord &record:msql.toResult())
            {
                QVariantMap info0;
                for(int i = 0; i < record.count(); i++)
                {
                    info0.insert(record.fieldName(i), record.value(i));
                }
                info.push_back(QJsonValue(QJsonObject::fromVariantMap(info0)));
            }
            jsonReturn.insert("info", QJsonValue(info));
        }
        else
        {
            HDL_DB_ERROR(jsonReturn)
                    logDbErr(&msql);
        }
        break;
      }
}

