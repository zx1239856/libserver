#ifndef HANDLE_H
#define HANDLE_H

#include <QObject>
#include "utils/handle/controlhdl.h"
#include "utils/exception.h"

class handle:public QObject
{
    Q_OBJECT
public:
    handle();
    handle(qint64 token);
    virtual ~handle() = 0;

protected:
    qint32 ID;
};

#endif // HANDLE_H
