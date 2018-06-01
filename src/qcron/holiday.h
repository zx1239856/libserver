#ifndef _HOLIDAY_H
#define _HOLIDAY_H

#include <QDate>

class Holiday
{
public:
    static QList<QDate> yearsHolidays(int year);
    static bool isHoliday(const QDate & date);
    static QDate easter(int year);
    static QDate next(const QDate & date);
};

#endif
