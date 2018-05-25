#include "operatebookhdl.h"

operatebookhdl::operatebookhdl(double token): handle(token){}

bool operatebookhdl::deal(const QString &command, const QJsonObject &json)
{
    QByteArray cpath = command.toLocal8Bit();
    char* cmd = cpath.data();

    QMetaEnum me = QMetaEnum::fromType<userhdl::CMD>();
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
