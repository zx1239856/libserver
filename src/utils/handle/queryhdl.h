#ifndef QUERYHDL_H
#define QUERYHDL_H

#include "utils/handle/handle.h"

class queryhdl: public handle
{
    Q_OBJECT

public:
    enum CMD
    {
        book,
        operationlog,
        borrowlog,
    };
    Q_ENUM(CMD)

    queryhdl(const QString& token);
    bool deal(const QString &cmd, const QJsonObject &json);
};

#endif // QUERYHDL_H
