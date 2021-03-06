#include "handle.h"

handle::handle(const QString& token): token(token)
{
    if(token == "unknown")
        ID = -1;
    else if(ctrl->ifLogin(token))
    {
        ID = ctrl->GetID(token).second;
        group = ctrl->GetID(token).first;
    }
    else
    {
        ID = -2;    //表示token错误，deal()直接返回错误json即可
    }
}

QJsonObject handle::GetReturn()
{
    return jsonReturn;
}
