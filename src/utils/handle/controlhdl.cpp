#include "controlhdl.h"

controlhdl* controlhdl::instance=nullptr;

controlhdl::controlhdl()
{

}

bool controlhdl::ifLogin(const QString& token)
{
    return mClient.contains(token);
}

QPair<int, int> controlhdl::GetID(const QString& token)
{
    return mClient[token];
}

void controlhdl::AddUser(const QString &token, QPair<int, int> ID)
{
    mClient.insert(token, ID);
}

controlhdl* controlhdl::getInstance()
{
  if(instance==nullptr)
  {
      instance = new controlhdl();
  }
  return instance;
}
