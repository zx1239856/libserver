#include "appointhdl.h"

using namespace sql;

appointhdl::appointhdl(const QString& token): handle(token){}

void appointhdl::deal(const QString &command, const QJsonObject &json)
{
    QByteArray cpath = command.toLocal8Bit();
    char* cmd = cpath.data();
    QMetaEnum me = QMetaEnum::fromType<appointhdl::CMD>();

    basicSQL* msql = nullptr;
    dbQueryThread dbQT;
    QVector<QSqlRecord> sqlresult;
    bool success;

    connect(&dbQT, &dbQueryThread::onResult, [&](const QVector<QSqlRecord> &res){
        success = true;
        sqlresult = res;
    });

    connect(&dbQT, &dbQueryThread::onSuccess, [&](){
        success = true;
    });

    connect(&dbQT, &dbQueryThread::onFail, [&](const QSqlError &err){
        success = false;
        qDebug() << err;
    });

    switch(me.keyToValue(cmd))
    {
    case appointborrow:
        if(ID > 0)
        {
            QMap<QString, QVariant> mAppoint;
            mAppoint.insert("readerid", ID);
            mAppoint.insert("type", "borrow");
            mAppoint.insert("bookid", json.value("id").toInt());
            mAppoint.insert("appointtime", json.value("appointtime").toString());
            msql = new sql::insert("libserver.lib_currappoint", mAppoint);
            dbQT.setSqlQuery(msql);
            dbQT.start();
            dbQT.wait();
            if(success)
                jsonReturn.insert("result", true);
            else
            {
                jsonReturn.insert("result", false);
                jsonReturn.insert("detail", "wrong database operation");
            }
        }
        else
        {
            jsonReturn.insert("result", false);
            jsonReturn.insert("detail", "wrong token");
        }
        break;

    case appointreturn:
        if(ID > 0)
        {
            QMap<QString, QVariant> mAppoint;
            mAppoint.insert("readerid", ID);
            mAppoint.insert("type", "return");
            mAppoint.insert("bookid", json.value("id").toInt());
            mAppoint.insert("appointtime", json.value("appointtime").toString());
            msql = new sql::insert("libserver.lib_currappoint", mAppoint);
            dbQT.setSqlQuery(msql);
            dbQT.start();
            dbQT.wait();
            if(success)
                jsonReturn.insert("result", true);
            else
            {
                jsonReturn.insert("result", false);
                jsonReturn.insert("detail", "wrong database operation");
            }
        }
        else
        {
            jsonReturn.insert("result", false);
            jsonReturn.insert("detail", "wrong token");
        }
        break;
    }

    dbQT.quit();
    if(msql)
        delete msql;
}
