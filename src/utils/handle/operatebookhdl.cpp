#include "operatebookhdl.h"

operatebookhdl::operatebookhdl(const QString& token): handle(token){}

void operatebookhdl::deal(const QString &command, const QJsonObject &json)
{
    QByteArray cpath = command.toLocal8Bit();
    char* cmd = cpath.data();

    QMetaEnum me = QMetaEnum::fromType<operatebookhdl::CMD>();
    switch(me.keyToValue(cmd))
    {
    case createbook:

        break;
    case deletebook:

        break;
    case changebook:

        break;
    case changegroup:

        break;
    }
}
