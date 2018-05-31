#include "queryhdl.h"

queryhdl::queryhdl(const QString& token): handle(token){}

void queryhdl::deal(const QString &command, const QJsonObject &json)
{
    QByteArray cpath = command.toLocal8Bit();
    char* cmd = cpath.data();

    QMetaEnum me = QMetaEnum::fromType<queryhdl::CMD>();
    switch(me.keyToValue(cmd))
    {
    case book:
        
        break;
    case operationlog:

        break;
    case borrowlog:

        break;
    }
}

