#include "logstream.h"
#include <QDebug>

LogStream::LogStream(const QString &filename): FileIO(filename, QIODevice::Append)
{
    if (success)
    {
        QString ready_string = "LogStream has been ready.";
        //qWarning() << ready_string << endl;
        *data_io << ready_string << endl;
    }
    else
    {
        qWarning() << "Error: CANNOT open logstream file.";
    }
}

void LogStream::operator <<(const QString &data)
{
    QDateTime current = QDateTime::currentDateTime();
    QString logString =current.toString("yyyy.MM.dd hh:mm:ss")
            + " " + data;
    qWarning() << logString;
    *data_io << logString << endl;
}

LogStream* LogStream::getInstance()
{
  return instance;
}
