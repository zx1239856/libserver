#include "controlhdl.h"

controlhdl* controlhdl::instance=nullptr;

controlhdl::controlhdl()
{

}

bool controlhdl::ifLogin(const QString& token)
{
    return mClient.contains(token);
}

qint32 controlhdl::GetID(const QString& token)
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
