#ifndef CONTROLHDL_H
#define CONTROLHDL_H
#include <QMap>
#include <QPair>

class controlhdl
{
    static controlhdl* instance;
    class controlhdlGarbo
    {
    public:
      ~controlhdlGarbo()
      {
        if(controlhdl::instance)delete controlhdl::instance;
      }
    };
    static controlhdlGarbo garbo;
    controlhdl();
    QMap<QString, QPair<int, int>> mClient;  //<token, ID>

public:
    controlhdl(const controlhdl&) = delete;
    controlhdl& operator=(controlhdl&) = delete;
    static controlhdl* getInstance();
    bool ifLogin(const QString& token);
    QPair<int, int> GetID(const QString& token);
    void AddUser(const QString &token, QPair<int, int> ID); //QPair<groupid, ID>
};

extern controlhdl* ctrl;

#endif // HANDLE_H
