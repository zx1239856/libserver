#include "controlhdl.h"

controlhdl::controlhdl()
{

}

bool controlhdl::ifLogin(qint64 token)
{
    return mClient.contains(token);
}

qint32 controlhdl::GetID(qint64 token)
{
    return mClient[token];
}

controlhdl* controlhdl::getInstance()
{
  if(instance==nullptr)
  {
      instance = new controlhdl();
  }
  return instance;
}
