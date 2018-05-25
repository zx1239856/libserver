#ifndef OPERATEUSERHDL_H
#define OPERATEUSERHDL_H

#include "utils/handle/handle.h"

class operateuserhdl: public handle
{
    Q_OBJECT

public:
    enum CMD
    {
        createuser,
        deleteuser,
        changegroup,
    };
    Q_ENUM(CMD)

    operateuserhdl(const QString& token);
    bool deal(const QString &cmd, const QJsonObject &json);
};

#endif // OPERATEUSERHDL_H
