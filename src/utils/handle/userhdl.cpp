#include "userhdl.h"

using namespace sql;

userhdl::userhdl(const QString& token): handle(token){}

void userhdl::deal(const QString &command, const QJsonObject &json)
{
    QByteArray cpath = command.toLocal8Bit();
    char* cmd = cpath.data();
    QMetaEnum me = QMetaEnum::fromType<userhdl::CMD>();

    basicSQL* msql = nullptr;
    QVector<QSqlRecord> sqlresult;
    bool success;

    switch(me.keyToValue(cmd))
    {
    case login:
        if(ID == -1)
        {
            msql = new sql::select("libserver.lib_" + json.value("group").toString(),
                              "username = '" + json.value("username").toString() + "' AND password = '" + json.value("password").toString() + "'");
            success = msql->exec();
            if(success)
            {
                sqlresult = msql->toResult();
                if(sqlresult.size() == 1)
                {
                    QString token = token::getToken(json.value("username").toString());
                    ctrl->AddUser(token,
                                  qMakePair(json.value("group").toString(), sqlresult[0].value("ID").toInt()));
                    jsonReturn.insert("result", true);
                    jsonReturn.insert("token", token);
                    qDebug() << ctrl->mClient;
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
        if(ID == -1)
        {
            msql = new sql::select("libserver.lib_" + json.value("group").toString(),
                              "username = '" + json.value("username").toString() + "' AND email = '" + json.value("auth").toString() + "'");
            success = msql->exec();
            sqlresult = msql->toResult();
            if(success && sqlresult.size() == 1)
            {
                //email
            }
            else
            {
                jsonReturn.insert("result", false);
                jsonReturn.insert("detail", "wrong username or email");
            }
        }
        else
        {
            jsonReturn.insert("result", false);
            jsonReturn.insert("detail", "wrong token");
        }
        break;
    case logout:
        if(ID >= 0)
        {
            ctrl->DeleteUser(token);
            jsonReturn.insert("result", true);
            qDebug() << ctrl->mClient;
        }
        else
        {
            jsonReturn.insert("result", false);
            jsonReturn.insert("detail", "wrong token");
        }
        break;
    case changepwd:
        if(ID > 0)
        {
            msql = new sql::select("libserver.lib_" + group,
                              "ID = " + QString::number(ID) + " AND  password = '" + json.value("oldpwd").toString() + "'");
            success = msql->exec();
            sqlresult = msql->toResult();
            if(success && sqlresult.size() > 0)
            {
                QMap<QString, QVariant> newpwd;
                newpwd.insert("password", json.value("newpwd").toString());
                update up("libserver.lib_" + group, newpwd, "ID = " + QString::number(ID));
                up.exec();
                jsonReturn.insert("result", true);
            }
            else
            {
                jsonReturn.insert("result", false);
                jsonReturn.insert("detail", "wrong old password");
            }
        }
        else
        {
            jsonReturn.insert("result", false);
            jsonReturn.insert("detail", "wrong token");
        }
        break;
    case updateinfo:
        if(ID > 0)
        {
            QMap<QString, QVariant> mInfo;
            QJsonArray info = json.value("info").toArray();
            for(auto iter = info.begin(); iter != info.end(); iter++)
            {
                mInfo.insert((*iter).toObject().value("field").toString(), (*iter).toObject().value("data").toString());
            }
            msql = new sql::update("libserver.lib_" + group, mInfo, "ID = " + QString::number(ID));
            success = msql->exec();
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
    if(msql)
        delete msql;
}
