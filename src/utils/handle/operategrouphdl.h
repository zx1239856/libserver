#ifndef OPERATEGROUPHDL_H
#define OPERATEGROUPHDL_H

#include "utils/handle/handle.h"

class operategrouphdl: public handle
{
    Q_OBJECT

public:
    enum CMD
    {
        createreader,
        createstaff,
        createbook,
        changereader,
        changestaff,
        changebook,
        deletereader,
        deletestaff,
        deletebook
    };
    Q_ENUM(CMD)

    operategrouphdl(double token);
    bool deal(const QString &cmd, const QJsonObject &json);
};
#endif // OPERATEGROUPHDL_H
