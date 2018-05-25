#include "appointhdl.h"

appointhdl::appointhdl(const QString& token): handle(token){}

bool appointhdl::deal(const QString &command, const QJsonObject &json)
{
    QByteArray cpath = command.toLocal8Bit();
    char* cmd = cpath.data();

    QMetaEnum me = QMetaEnum::fromType<appointhdl::CMD>();
    switch(me.keyToValue(cmd))
    {
    case appointborrow:

        break;
    case appointreturn:

        break;
    }
}
