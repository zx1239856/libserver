#include "operateuserhdl.h"

operateuserhdl::operateuserhdl(const QString& token): handle(token){}

void operateuserhdl::deal(const QString &command, const QJsonObject &json)
{
    QByteArray cpath = command.toLocal8Bit();
    char* cmd = cpath.data();

    QMetaEnum me = QMetaEnum::fromType<operateuserhdl::CMD>();
    switch(me.keyToValue(cmd))
    {
    case createuser:

        break;
    case deleteuser:

        break;
    case changegroup:

        break;
    }
}
