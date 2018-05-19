#ifndef USERHDL_H
#define USERHDL_H
#include "utils/handle/handle.h"

class userhdl: public handle
{
    Q_OBJECT
public:
    enum CMD
    {
        login,
        forget,
        logout,
        changepwd,
        updateinfo
    };
    Q_ENUM(CMD)

    userhdl(qint64 token);
    virtual ~userhdl();
    qint32 deal(CMD cmd, QString username, QString password);
};

#endif // USERHDL_H
