#include "queryhdl.h"

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
    case book:
        if(json.value("rule").toString() == "completesearch")
        {
            QJsonArray cond = json.value("info").toArray();
            QString conditions;
            for(auto y : cond)
            {
                auto x = y.toObject();
                if(x.value("field").toString() == "tags")
                {
                    QJsonArray tags = x.value("data").toArray();
                    for(auto iter:tags)
                    {
                        conditions += ("tags LIKE '%," +iter.toString() + ",%' AND ");
                    }
                }
                else if(x.value("field").toString() == "bookcase")
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
                else if(x.value("data").isString())
                    conditions += x.value("field").toString() + " = '" + x.value("data").toString() + "' AND ";
                else if(x.value("data").isDouble())
                    conditions += x.value("field").toString() + " = " + QString::number(x.value("data").toDouble()) + " AND ";
                else if(x.value("data").isBool())
                    conditions += x.value("field").toString() + " = " + QString::number(x.value("data").toBool()) + " AND ";
            }
            if(!conditions.isNull())
                conditions.chop(5);

            //添加排序
            conditions += " ORDER BY ";
            for(const QJsonValue &x:json.value("order").toArray())
            {
                conditions += x.toString() + ",";
            }
            conditions.chop(1);

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
                if(x.value("field").toString() == "tags")
                {
                    QJsonArray tags = x.value("data").toArray();
                    for(auto iter:tags)
                    {
                        conditions += ("tags LIKE '%" +iter.toString() + "%' AND ");
                    }
                }
                else if(x.value("field").toString() == "bookcase")
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
                else if(x.value("data").isString())
                    conditions += x.value("field").toString() + " LIKE '%" + x.value("data").toString() + "%' AND ";
                else if(x.value("data").isDouble())
                    conditions += x.value("field").toString() + " = " + QString::number(x.value("data").toDouble()) + " AND ";
                else if(x.value("data").isBool())
                    conditions += x.value("field").toString() + " = " + QString::number(x.value("data").toBool()) + " AND ";
            }
            if(!conditions.isNull())
                conditions.chop(5);

            //添加排序
            conditions += " ORDER BY ";
            for(const QJsonValue &x:json.value("order").toArray())
            {
                conditions += x.toString() + ",";
            }
            conditions.chop(1);

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
            QString info = json.value("info").toString();
            conditions += "name LIKE '%" + info +"%' OR author LIKE '%" + info + "%' OR press LIKE '%" + info + "%' OR tags LIKE '%" + info + "%'";

            //添加排序
            conditions += " ORDER BY ";
            for(const QJsonValue &x:json.value("order").toArray())
            {
                conditions += x.toString() + ",";
            }
            conditions.chop(1);

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
        QString table = json.value("table").toString();
        //添加排序
        conditions += " ORDER BY ";
        for(const QJsonValue &x:json.value("order").toArray())
        {
            conditions += x.toString() + ",";
        }
        conditions.chop(1);

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

