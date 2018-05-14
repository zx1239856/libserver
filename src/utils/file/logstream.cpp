#include "logstream.h"
#include <QDebug>
QTextStream cout(stdout,  QIODevice::WriteOnly);

QString LogStream::logpath="server.log";
LogStream* LogStream::instance = nullptr;

LogStream::LogStream(const QString &filename): FileIO(filename, QIODevice::Append)
{
    if (success)
    {
        QString ready_string = "LogStream has been ready.";
        cout << ready_string << endl;
        *data_io << ready_string << endl;
    }
    else
    {
        throw("Error: CANNOT open logstream file.");
    }
}

void LogStream::operator <<(const QString &data)
{
    QDateTime current = QDateTime::currentDateTime();
    QString logString =current.toString("yyyy.MM.dd hh:mm:ss")
            + " " + data;
    //qWarning() << logString;
    cout << logString << endl;
    *data_io << logString << endl;
}

void LogStream::setPath(const QString &p)
{
  logpath = p;
}

LogStream* LogStream::getInstance()
{
  if(instance==nullptr)
    {
      instance = new LogStream(logpath);
    }
  return instance;
}
