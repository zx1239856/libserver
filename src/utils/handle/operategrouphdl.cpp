#include "operategrouphdl.h"

operategrouphdl::operategrouphdl(double token): handle(token){}

bool operategrouphdl::deal(const QString &command, const QJsonObject &json)
{
    QByteArray cpath = command.toLocal8Bit();
    char* cmd = cpath.data();

    QMetaEnum me = QMetaEnum::fromType<userhdl::CMD>();
    switch(me.keyToValue(cmd))
    {
    case createreader:

        break;
    case createstaff:

        break;
    case createbook:

        break;
    case changereader:

        break;
    case changestaff:

        break;
    case changebook:

        break;
    case deletereader:

        break;
    case deletestaff:

        break;
    case deletebook:

        break;
    }
}
