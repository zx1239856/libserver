#include "queryhdl.h"

queryhdl::queryhdl(double token): handle(token){}

bool queryhdl::deal(const QString &command, const QJsonObject &json)
{
    QByteArray cpath = command.toLocal8Bit();
    char* cmd = cpath.data();

    QMetaEnum me = QMetaEnum::fromType<userhdl::CMD>();
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

