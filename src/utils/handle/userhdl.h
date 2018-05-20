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

    userhdl(double token);
    bool deal(const QString &cmd, const QJsonObject &json);
};

#endif // USERHDL_H
