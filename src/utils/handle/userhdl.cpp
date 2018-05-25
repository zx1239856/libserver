#include "userhdl.h"

using namespace sql;

userhdl::userhdl(const QString& token): handle(token){}

void userhdl::deal(const QString &command, const QJsonObject &json)
{
    QByteArray cpath = command.toLocal8Bit();
    char* cmd = cpath.data();
    QMetaEnum me = QMetaEnum::fromType<userhdl::CMD>();

    basicSQL* msql;
    dbQueryThread dbQT(msql);
    QVector<QSqlRecord> sqlresult;
    bool success;

    connect(&dbQT, &dbQueryThread::onResult, [&](const QVector<QSqlRecord> &res){
        success = true;
        sqlresult = res;
    });

    connect(&dbQT, &dbQueryThread::onFail, [&](){
        success = false;
    });

    switch(me.keyToValue(cmd))
    {
    case login:
        if(ID == -1)
        {
            msql = new sql::select("libserver.lib_" + json.value("group").toString(),
                              "name = '" + json.value("username").toString() + "' AND password = '" + json.value("password").toString() + "'");
            dbQT.start();
            dbQT.wait();
            if(success)
            {
                if(sqlresult.size() > 0)
                {
                    QString token = token::getToken(json.value("username").toString());
                    ctrl->AddUser(token,
                                  qMakePair(json.value("group").toInt(), json.value("ID").toInt()));
                    jsonReturn.insert("result", true);
                    jsonReturn.insert("token", token);
                }
                else
                {
                    jsonReturn.insert("result", false);
                    jsonReturn.insert("detail", "wrong password");
                }
            }
            else
            {
                jsonReturn.insert("result", false);
                jsonReturn.insert("detail", "wrong username");
            }
        }
        else
        {
            jsonReturn.insert("result", false);
            jsonReturn.insert("detail", "wrong token");
        }
        break;
    case forget:

        break;
    case logout:

        break;
    case changepwd:

        break;
    case updateinfo:

        break;
    }

    dbQT.quit();
    delete msql;
}
