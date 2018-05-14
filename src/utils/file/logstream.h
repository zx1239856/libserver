#ifndef LOGSTREAM_H
#define LOGSTREAM_H

#include "fileio.h"
#include <QFile>
#include <QString>
#include <QTextStream>
#include <QDateTime>

class LogStream : public FileIO
{
  static QString logpath;
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
  LogStream(const QString &filename);
public:
    LogStream(const LogStream&)=delete;
    LogStream& operator=(LogStream&)=delete;
    ~LogStream();
    static LogStream* getInstance();
    static void setPath(const QString &p);
    void operator <<(const QString &data);
};

//lstream是一个在main.cpp中声明的全局变量
extern LogStream* lstream;

#endif // LOGSTREAM_H
