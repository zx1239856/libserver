#include "checkhdl.h"
#include <QDate>
using namespace sql;
using namespace globalInfo;

checkhdl::checkhdl(const QString& token): handle(token){}

void checkhdl::deal(const QString &command, const QJsonObject &json)
{
    QByteArray cpath = command.toLocal8Bit();
    char* cmd = cpath.data();
    QMetaEnum me = QMetaEnum::fromType<checkhdl::CMD>();

    // update token status
    ctrl->UpdateStatus(token);

    switch(me.keyToValue(cmd))
    {
    case checkborrow:
        if(ID > 0)
        {
            if(group!="staffs")
            {
                HDL_PERM_DENIED(jsonReturn)
            }
            else
            {
                sql::select msql1(dbFullPrefix + "currappoint", "ID = " + QString::number(json.value("appointid").toInt()));
                if(msql1.exec() && msql1.toResult().size() == 1)
                {
                    auto sqlresult1 = msql1.toResult()[0];
                    if(sqlresult1.value("type").toString() == "borrow")
                    {
                        QMap<QString, QVariant> map;
                        map.insert("readerid", sqlresult1.value("readerid").toInt());
                        map.insert("bookid", sqlresult1.value("bookid").toInt());
                        map.insert("operatorid", ID);
                        sql::select msql2(dbFullPrefix + "readers", "ID = " + QString::number(sqlresult1.value("readerid").toInt()));

                        if(msql2.exec() && msql2.toResult().size() == 1)
                        {
                            auto sqlresult2 = msql2.toResult()[0];
                            sql::select msql3(dbFullPrefix + "readergroup", "ID = " + QString::number(sqlresult2.value("groupid").toInt()));
                            if(msql3.exec() && msql3.toResult().size() == 1)
                            {
                                auto sqlresult3 = msql3.toResult()[0];
                                int max_borrow_time = sqlresult3.value("max_borrow_time").toInt();
                                int max_renew_time = sqlresult3.value("max_renew_time").toInt();
                                map.insert("remaintime", max_renew_time);
                                map.insert("exptime", QDate::currentDate().addDays(max_borrow_time).toString("yyyy-MM-dd"));
                                map.insert("ID", 0);
                                sql::insert msql4(dbFullPrefix + "currborrow", map);
                                if(msql4.exec())
                                {
                                    sql::del msql5(dbFullPrefix + "currappoint", "ID = " + QString::number(json.value("appointid").toInt()));
                                    QMap<QString, QVariant> content;
                                    content.insert("available", false);
                                    sql::update msql6(dbFullPrefix + "books", content, " ID = " + QString::number(sqlresult1.value("bookid").toInt()));
                                    msql5.exec();
                                    msql6.exec();
                                    dbLog::log("checkborrow","The admin checked borrow, BookID="+json.value("bookid").toString()+",ReaderID="+json.value("readerid").toString(),ID,dbLog::staff);
                                    HDL_SUCCESS(jsonReturn)
                                }
                                else
                                {
                                    HDL_DB_ERROR(jsonReturn)
                                    logDbErr(&msql4);
                                }
                            }
                            else
                            {
                                HDL_DB_ERROR(jsonReturn)
                                logDbErr(&msql3);
                            }
                        }
                        else
                        {
                            HDL_DB_ERROR(jsonReturn)
                            logDbErr(&msql2);
                        }
                    }
                    else
                    {
                        jsonReturn.insert("result", false);
                        jsonReturn.insert("detail", "invalid appoint type");
                    }
                }
                else
                {
                    HDL_DB_ERROR(jsonReturn)
                    logDbErr(&msql1);
                }
            }
        }
        else
        {
            HDL_INV_TOKEN(jsonReturn)
        }
        break;
    case checkreturn:
        if(ID > 0)
        {
            if(group!="staffs")
            {
                HDL_PERM_DENIED(jsonReturn)
            }
            else
            {
                sql::select msql1(dbFullPrefix + "currappoint", "ID = " + QString::number(json.value("appointid").toInt()));
                if(msql1.exec() && msql1.toResult().size() == 1)
                {
                    auto sqlresult1 = msql1.toResult()[0];
                    if(sqlresult1.value("type").toString() == "return")
                    {
                        sql::del msql2(dbFullPrefix + "currborrow", "ID = " + QString::number(sqlresult1.value("borrowid").toInt()));
                        if(msql2.exec())
                        {
                            sql::del msql3(dbFullPrefix + "currappoint", "ID = " + QString::number(json.value("appointid").toInt()));
                            QMap<QString, QVariant> content;
                            content.insert("available", true);
                            sql::update msql4(dbFullPrefix + "books", content, " ID = " + QString::number(sqlresult1.value("bookid").toInt()));
                            msql3.exec();
                            msql4.exec();
                            dbLog::log("checkreturn","The admin checked return, borrowID="+json.value("borrowid").toString()+",appoint ID="+json.value("appointid").toString(),ID,dbLog::staff);
                            HDL_SUCCESS(jsonReturn)
                        }
                        else
                        {
                            HDL_DB_ERROR(jsonReturn)
                            logDbErr(&msql2);
                        }
                    }
                    else
                    {
                        jsonReturn.insert("result", false);
                        jsonReturn.insert("detail", "invalid appoint type");
                    }
                }
                else
                {
                    HDL_DB_ERROR(jsonReturn)
                    logDbErr(&msql1);
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
