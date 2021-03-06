#ifndef OPERATEBOOKHDL_H
#define OPERATEBOOKHDL_H

#include "utils/handle/handle.h"

class operatebookhdl: public handle
{
    Q_OBJECT

public:
    enum CMD
    {
        createbook,
        deletebook,
        changebook,
        changegroup
    };
    Q_ENUM(CMD)

    operatebookhdl(const QString& token);
    void deal(const QString &cmd, const QJsonObject &json);
};

#endif // OPERATEBOOKHDL_H
