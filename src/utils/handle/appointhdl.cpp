#include "appointhdl.h"

appointhdl::appointhdl(double token): handle(token){}

bool appointhdl::deal(const QString &command, const QJsonObject &json)
{
    QByteArray cpath = command.toLocal8Bit();
    char* cmd = cpath.data();

    QMetaEnum me = QMetaEnum::fromType<userhdl::CMD>();
    switch(me.keyToValue(cmd))
    {
    case appointborrow:

        break;
    case appointreturn:

        break;
    }
}
