#include "logstream.h"
static QTextStream cout(stdout, QIODevice::WriteOnly);

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
        cout << "Error: CANNOT open logstream file."<< endl;
    }
}

void LogStream::operator <<(const QString &data)
{
    QDateTime current = QDateTime::currentDateTime();
    QString logString =current.toString("yyyy.MM.dd hh:mm:ss")
            + " " + data;
    cout << logString << endl;
    *data_io << logString << endl;
}
