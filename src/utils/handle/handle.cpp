#include "handle.h"

handle::handle(double token)
{
    if(token == -1)
        ID = -1;
    else
    {
        if(!ctrl->ifLogin(token))
            throw(FalseToken);
        ID = ctrl->GetID(token);
    }
}

QJsonObject handle::GetReturn()
{
    return jsonReturn;
}