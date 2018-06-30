#ifndef DOWNLOADHDL_H
#define DOWNLOADHDL_H

#include "utils/handle/handle.h"

class downloadhdl: public handle
{
    Q_OBJECT

public:
    enum CMD
    {
        checkborrow,
        checkreturn,
    };
    Q_ENUM(CMD)

    checkhdl(const QString& token);
    void deal(const QString &cmd, const QJsonObject &json);
};

#endif // DOWNLOADHDL_H
