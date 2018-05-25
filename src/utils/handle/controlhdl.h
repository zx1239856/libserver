#ifndef CONTROLHDL_H
#define CONTROLHDL_H
#include <QMap>

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
    QMap<QString, qint32> mClient;  //<token, ID>

public:
    controlhdl(const controlhdl&) = delete;
    controlhdl& operator=(controlhdl&) = delete;
    static controlhdl* getInstance();
    bool ifLogin(const QString& token);
    qint32 GetID(const QString& token);

};

extern controlhdl* ctrl;

#endif // HANDLE_H
