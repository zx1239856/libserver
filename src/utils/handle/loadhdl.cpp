#include "loadhdl.h"

using namespace globalInfo;

loadhdl::loadhdl(const QString& token): handle(token){}

void loadhdl::deal(const QString &command, const QJsonObject &json)
{
    QByteArray cpath = command.toLocal8Bit();
    char* cmd = cpath.data();
    QMetaEnum me = QMetaEnum::fromType<loadhdl::CMD>();

    int bookid = json.value("id").toInt();
    switch(me.keyToValue(cmd))
    {
    case down:
    {
        sql::select msql(dbFullPrefix + "books", "ID = " + QString::number(bookid));
        if(msql.exec() && msql.toResult().size() == 1)
        {
            auto sqlresult = msql.toResult()[0];
            if(!sqlresult.value(json.value("object").toString()).isNull())
            {
                QString fileID = json.value("object").toString() + "/" + sqlresult.value(json.value("object").toString()).toString();
                HDL_SUCCESS(jsonReturn);
                jsonReturn.insert("mode", "down");
                jsonReturn.insert("file", fileID);
            }
            else
            {
                jsonReturn.insert("result", false);
                jsonReturn.insert("detail", "NULL FILE");
            }
        }
        else
        {
            HDL_DB_ERROR(jsonReturn);
        }
    }
        break;
    case up:
    {
        sql::select msql(dbFullPrefix + "books", "ID = " + QString::number(bookid));
        if(msql.exec() && msql.toResult().size() == 1)
        {
            QString fileID = json.value("object").toString() + "/" + token::getMD5(msql.toResult()[0].value("ISBN").toString());
            QMap<QString, QVariant> sqlMap = {std::make_pair(json.value("object").toString(), fileID)};
            sql::update msql1(dbFullPrefix + "books", sqlMap, "ID = " + QString::number(bookid));
            msql1.exec();
            HDL_SUCCESS(jsonReturn);
            jsonReturn.insert("mode", "up");
            jsonReturn.insert("file", fileID);
        }
        else
        {
            HDL_DB_ERROR(jsonReturn);
        }
    }
    }
}
