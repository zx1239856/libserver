#include "controlhdl.h"

controlhdl* controlhdl::instance=nullptr;

controlhdl::controlhdl()
{

}

bool controlhdl::ifLogin(const QString& token)
{
    return mClient.contains(token);
}

QPair<QString, int> controlhdl::GetID(const QString& token)
{
    return mClient[token];
}

void controlhdl::AddUser(const QString &token, const QPair<QString, int> &ID)
{
    mClient.insert(token, ID);
}

void controlhdl::DeleteUser(const QString &token)
{
    mClient.erase(mClient.find(token));
}

controlhdl* controlhdl::getInstance()
{
  if(instance==nullptr)
  {
      instance = new controlhdl();
  }
  return instance;
}
