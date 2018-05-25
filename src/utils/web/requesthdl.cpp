#include "requesthdl.h"

requesthdl::requesthdl(const QByteArray &rqtData)
{
    QJsonDocument jsonDocument = QJsonDocument::fromBinaryData(rqtData);
    jsonRequest = jsonDocument.object();
}

QByteArray requesthdl::deal()
{
    handle* hdl;
    if(jsonRequest.value("type").toString() == "USER")
    {
        hdl = new userhdl(jsonRequest.value("token").toString());
    }
    else if(jsonRequest.value("type").toString() == "QUERY")
    {

    }
    else if(jsonRequest.take("type").toString() == "APPOINT")
    {

    }
    else if(jsonRequest.take("type").toString() == "CHECK")
    {

    }
    else if(jsonRequest.take("type").toString() == "OPERATEUSER")
    {

    }
    else if(jsonRequest.take("type").toString() == "OPERATEBOOK")
    {

    }
    else if(jsonRequest.take("type").toString() == "OPERATEGROUP")
    {

    }
    else if(jsonRequest.take("type").toString() == "OPERATE")
    {

    }
    else
        throw FailedAnalyze;

    hdl->deal(jsonRequest.take("command").toString(), jsonRequest);

    QJsonDocument jdocReturn(hdl->GetReturn());
    delete hdl;
    return jdocReturn.toBinaryData();
}
