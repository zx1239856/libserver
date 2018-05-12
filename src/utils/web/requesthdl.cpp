#include "requesthdl.h"

requesthdl::requesthdl(const QByteArray &rqtData)
{
    QJsonDocument jsonDocument = QJsonDocument::fromBinaryData(rqtData);
    jsonRequest = jsonDocument.object();
}

QByteArray requesthdl::deal()
{

    //
    QJsonDocument jdocReturn(jsonReturn);
    return jdocReturn.toBinaryData();
}
