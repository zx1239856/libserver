#ifndef LOGSTREAM_H
#define LOGSTREAM_H

#include "fileio.h"
#include <QFile>
#include <QString>
#include <QTextStream>
#include <QDateTime>

class LogStream : public FileIO
{
public:
    LogStream(const QString &filename = "Server.log");
    ~LogStream();
    void operator <<(const QString &data);
};

//lstream是一个在main.cpp中声明的全局变量
extern LogStream* lstream;

#endif // LOGSTREAM_H
