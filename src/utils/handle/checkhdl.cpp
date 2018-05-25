#include "checkhdl.h"

checkhdl::checkhdl(const QString& token): handle(token){}

bool checkhdl::deal(const QString &command, const QJsonObject &json)
{
    QByteArray cpath = command.toLocal8Bit();
    char* cmd = cpath.data();

    QMetaEnum me = QMetaEnum::fromType<checkhdl::CMD>();
    switch(me.keyToValue(cmd))
    {
    case checkborrow:

        break;
    case checkreturn:

        break;
    }
}
