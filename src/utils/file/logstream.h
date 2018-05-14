#ifndef LOGSTREAM_H
#define LOGSTREAM_H

#include "fileio.h"
#include <QFile>
#include <QString>
#include <QTextStream>
#include <QDateTime>

class LogStream : public FileIO
{
  /*
  static LogStream *instance;
  class LogStreamGarbo
  {
  public:
    ~LogStreamGarbo()
    {
      if(LogStream::instance)delete LogStream::instance;
    }
  };
  static LogStreamGarbo garbo;
  */

public:
    LogStream(const QString &filename = "Server.log");
    ~LogStream();
    //static LogStream* getInstance();
    void operator <<(const QString &data);
};

//lstream是一个在main.cpp中声明的全局变量
extern LogStream* lstream;

#endif // LOGSTREAM_H
