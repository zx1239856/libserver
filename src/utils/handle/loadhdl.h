#ifndef LOADHDL_H
#define LOADHDL_H

#include "utils/handle/handle.h"
#include "utils/crypto/token.h"

class loadhdl: public handle
{
    Q_OBJECT

public:
    enum CMD
    {
        up,
        down,
    };
    Q_ENUM(CMD)

    loadhdl(const QString& token);
    void deal(const QString &cmd, const QJsonObject &json);

};

#endif // LOADHDL_H
