#include "userhdl.h"

userhdl::userhdl(const QString& token): handle(token){}

bool userhdl::deal(const QString &command, const QJsonObject &json)
{
    QByteArray cpath = command.toLocal8Bit();
    char* cmd = cpath.data();

    QMetaEnum me = QMetaEnum::fromType<userhdl::CMD>();
    switch(me.keyToValue(cmd))
    {
    case login:
        break;
    case forget:

        break;
    case logout:

        break;
    case changepwd:

        break;
    case updateinfo:

        break;
    }
}
