#include "handle.h"

handle::handle(qint64 token)
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

handle::~handle()
{

}
