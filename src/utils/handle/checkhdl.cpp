#include "checkhdl.h"

checkhdl::checkhdl(const QString& token): handle(token){}

void checkhdl::deal(const QString &command, const QJsonObject &json)
{
    QByteArray cpath = command.toLocal8Bit();
    char* cmd = cpath.data();
    QMetaEnum me = QMetaEnum::fromType<checkhdl::CMD>();

    sql::basicSQL* msql = nullptr;
    bool success;

    switch(me.keyToValue(cmd))
    {
    case checkborrow:
        if(ID > 0)
        {
            msql = new sql::select("libserver.lib_curappoint", "id = " + json.value("appointid").toString());
            success = msql->exec();
            /*if(success && sqlresult.size() == 1 && sqlresult[0].value(""))
            {

                QMap<QString, QVariant> mAppoint;
                mAppoint.insert("readerid", json.value("").toInt());
                //mAppoint.insert("bookid", );
                mAppoint.insert("borrowtime", json.value("id").toString());
                mAppoint.insert("exptime", json.value("appointtime").toString());
                mAppoint.insert("remaintime", json.value("appointtime").toInt());
                msql = new sql::insert("libserver.lib_curappoint", mAppoint);
                jsonReturn.insert("result", true);
            }
            else
            {
                jsonReturn.insert("result", false);
                jsonReturn.insert("detail", "wrong database operation");
            }*/
        }
        else
        {
            jsonReturn.insert("result", false);
            jsonReturn.insert("detail", "wrong token");
        }
        break;
    case checkreturn:

        break;
    }
}
