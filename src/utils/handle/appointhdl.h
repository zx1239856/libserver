#ifndef APPOINTHDL_H
#define APPOINTHDL_H

#include "utils/handle/handle.h"

class appointhdl: public handle
{
    Q_OBJECT

public:
    enum CMD
    {
        appointborrow,
        appointreturn,
    };
    Q_ENUM(CMD)

    appointhdl(const QString& token);
    bool deal(const QString &cmd, const QJsonObject &json);
};

#endif // APPOINTHDL_H
