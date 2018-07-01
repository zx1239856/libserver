#include <QFileInfo>
#include "userhdl.h"
#include "utils/smtp/sendemail.h"
#include "globalInfo.h"
#include "qdaemonlog.h"

using namespace sql;
using namespace globalInfo;

userhdl::userhdl(const QString& token): handle(token){}

void userhdl::deal(const QString &command, const QJsonObject &json)
{
    QByteArray cpath = command.toLocal8Bit();
    char* cmd = cpath.data();
    QMetaEnum me = QMetaEnum::fromType<userhdl::CMD>();

    basicSQL* msql = nullptr;
    QVector<QSqlRecord> sqlresult;
    // update token status
    ctrl->UpdateStatus(token);
    switch(me.keyToValue(cmd))
    {
    case login:
        if(ID == -1) // check token
        {
            dbLog::operatorType userType = (json.value("group").toString()=="staffs")?dbLog::staff:dbLog::reader;
            msql = new sql::select(dbFullPrefix + json.value("group").toString(),
                                   "username = '" + json.value("username").toString()+"'");
            if(msql->exec())  // db okay
            {
                sqlresult = msql->toResult();
                if(sqlresult.size() == 1) // user found
                {
                    if(sqlresult[0].value("password").toString()==json.value("password").toString()) // pwd valid
                    {
                        QString token = token::getToken(json.value("username").toString());
                        ctrl->AddUser(token,
                                      qMakePair(json.value("group").toString(), sqlresult[0].value("ID").toInt()));
                        jsonReturn.insert("result", true);
                        jsonReturn.insert("token", token);
                        dbLog::log("userLogin","User "+json.value("username").toString()+" login",ID,userType);
                    }
                    else // wrong pwd
                    {
                        jsonReturn.insert("result", false);
                        jsonReturn.insert("detail", "wrong password");
                    }
                }
                else // not found
                {
                    jsonReturn.insert("result", false);
                    jsonReturn.insert("detail", "user does not exist");
                }
            }
            else
            {
                // server error, log
                HDL_DB_ERROR(jsonReturn)
                logDbErr(msql);
            }
        }
        else
        {
            HDL_INV_TOKEN(jsonReturn)
        }
        break;
    case forget:
        if(ID == -1)
        {
            msql = new sql::select(dbFullPrefix + json.value("group").toString(),
                                   "username = '" + json.value("username").toString() + "' AND email = '" + json.value("auth").toString() + "'");
            if(msql->exec())
            {
                sqlresult = msql->toResult();
                if(sqlresult.size() == 1)
                {
                    QString newpwd = token::getToken(json.value("username").toString());
                    QMap<QString, QVariant> map;
                    map.insert("password", token::getMD5(newpwd));
                    sql::update updatepwd(dbFullPrefix + json.value("group").toString(), map, "username = '" + json.value("username").toString() + "' AND email = '" + json.value("auth").toString() + "'");
                    if(updatepwd.exec())
                    {
                        QString emailContent = emailChangePwdContent(newpwd);
                        // find email template
                        QString tplPath = config::getInstance()->templateDir() + "/forgetPwd.html";
                        QFileInfo tpl(tplPath);
                        if(tpl.isFile())
                        {
                            QFile file(tplPath);
                            if (file.open(QIODevice::ReadOnly | QIODevice::Text))
                            {
                                QTextStream in(&file);
                                emailContent = in.readAll();
                                emailContent.replace("%%Servicename",appName);
                                emailContent.replace("%%Newpwd",newpwd);
                            }
                        }
                        QStringList rcp(json.value("auth").toString());
                        sendEmail email(*config::getInstance(), rcp, emailChangePwdTitle, emailContent);
                        dbLog::log("userForgetPWD","Attempting to reset password, email="+json.value("auth").toString(),0,dbLog::general);
                        QObject::connect(&email,&sendEmail::onFail,this,[&](const QString& what)
                        {
                            qDaemonLog(what,QDaemonLog::ErrorEntry);
                            jsonReturn.insert("result", false);
                            jsonReturn.insert("detail", what);
                        });
                        QObject::connect(&email,&sendEmail::onSuccess,this,[&]()
                        {
                            HDL_SUCCESS(jsonReturn);
                        });
                        email.send();
                    }
                    else
                    {
                        HDL_DB_ERROR(jsonReturn)
                        logDbErr(&updatepwd);
                    }
                }
                else
                {
                    jsonReturn.insert("result", false);
                    jsonReturn.insert("detail", "Wrong username or email");
                }
            }
            else
            {
                HDL_DB_ERROR(jsonReturn)
                logDbErr(msql);
            }
        }
        else
        {
            HDL_INV_TOKEN(jsonReturn)
        }
        break;
    case logout:
        if(ID >= 0)
        {
            ctrl->DeleteUser(token);
            jsonReturn.insert("result", true);
            dbLog::log("userLogout","User logout",ID,dbLog::general);
        }
        else
        {
            HDL_INV_TOKEN(jsonReturn)
        }
        break;
    case changepwd:
        if(ID > 0)
        {
            msql = new sql::select(dbFullPrefix + group,
                                   "ID = " + QString::number(ID) + " AND  password = '" + json.value("oldpwd").toString() + "'");
            if(msql->exec())
            {
                sqlresult = msql->toResult();
                if(sqlresult.size() > 0)
                {
                    QMap<QString, QVariant> newpwd;
                    newpwd.insert("password", json.value("newpwd").toString());
                    update up(dbFullPrefix + group, newpwd, "ID = " + QString::number(ID));
                    up.exec();
                    dbLog::log("userChangePWD","User changed password",ID,(group=="staffs")?dbLog::staff:dbLog::reader);
                    jsonReturn.insert("result", true);
                }
                else
                {
                    jsonReturn.insert("result", false);
                    jsonReturn.insert("detail", "Wrong old password");
                }
            }
            else
            {
                HDL_DB_ERROR(jsonReturn)
                logDbErr(msql);
            }
        }
        else
        {
            HDL_INV_TOKEN(jsonReturn)
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
            msql = new sql::update(dbFullPrefix + group, mInfo, "ID = " + QString::number(ID));
            if(msql->exec())
              {
                dbLog::log("userUpdateInfo","User updated personal info",ID,(group=="staffs")?dbLog::staff:dbLog::reader);
                jsonReturn.insert("result", true);
              }
            else
            {
                HDL_DB_ERROR(jsonReturn)
                logDbErr(msql);
            }
        }
        else
        {
            HDL_INV_TOKEN(jsonReturn)
        }
        break;
    }
    if(msql)
        delete msql;
}
