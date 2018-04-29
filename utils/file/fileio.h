#ifndef FILEIO_H
#define FILEIO_H

#include <QString>
#include <QFile>
#include <QTextStream>

class FileIO
{
protected:
    QFile* data;
    QTextStream* data_io;
    bool success = false;

public:
    FileIO(const QString &filename,const QIODevice::OpenModeFlag &mode);
    ~FileIO();
};
#endif // FILEIO_H
