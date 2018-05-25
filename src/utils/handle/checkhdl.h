#ifndef CHECKHDL_H
#define CHECKHDL_H

#include "utils/handle/handle.h"

class checkhdl: public handle
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

#endif // APPOINTHDL_H
