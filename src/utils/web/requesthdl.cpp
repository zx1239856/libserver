#include "requesthdl.h"
#include "utils/handle/userhdl.h"
#include "utils/handle/queryhdl.h"
#include "utils/handle/checkhdl.h"
#include "utils/handle/operatebookhdl.h"
#include "utils/handle/operategrouphdl.h"
#include "utils/handle/operateuserhdl.h"
#include "utils/handle/appointhdl.h"

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
        hdl = new queryhdl(jsonRequest.value("token").toString());
    }
    else if(jsonRequest.value("type").toString() == "APPOINT")
    {
        hdl = new appointhdl(jsonRequest.value("token").toString());
    }
    else if(jsonRequest.value("type").toString() == "CHECK")
    {
        hdl = new checkhdl(jsonRequest.value("token").toString());
    }
    else if(jsonRequest.value("type").toString() == "OPERATEUSER")
    {
        hdl = new operateuserhdl(jsonRequest.value("token").toString());
    }
    else if(jsonRequest.value("type").toString() == "OPERATEBOOK")
    {
        hdl = new operatebookhdl(jsonRequest.value("token").toString());
    }
    else if(jsonRequest.value("type").toString() == "OPERATEGROUP")
    {
        hdl = new operategrouphdl(jsonRequest.value("token").toString());
    }
    else if(jsonRequest.value("type").toString() == "OPERATE")
    {
        //hdl = new operatehdl(jsonRequest.value("token").toString());
    }
    else
    {

    }

    hdl->deal(jsonRequest.take("command").toString(), jsonRequest);

    QJsonDocument jdocReturn(hdl->GetReturn());
    delete hdl;
    return jdocReturn.toBinaryData();
}
