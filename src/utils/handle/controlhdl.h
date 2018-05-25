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

public:
    QMap<QString, QPair<QString, int>> mClient;  //<token, ID>
    controlhdl(const controlhdl&) = delete;
    controlhdl& operator=(controlhdl&) = delete;
    static controlhdl* getInstance();
    bool ifLogin(const QString& token);
    QPair<QString, int> GetID(const QString& token);
    void AddUser(const QString &token, const QPair<QString, int> &ID); //QPair<groupid, ID>
    void DeleteUser(const QString &token);
};

extern controlhdl* ctrl;

#endif // HANDLE_H
