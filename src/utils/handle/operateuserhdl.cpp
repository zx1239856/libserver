#include "operateuserhdl.h"

operateuserhdl::operateuserhdl(double token): handle(token){}

bool operateuserhdl::deal(const QString &command, const QJsonObject &json)
{
    QByteArray cpath = command.toLocal8Bit();
    char* cmd = cpath.data();

    QMetaEnum me = QMetaEnum::fromType<userhdl::CMD>();
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
