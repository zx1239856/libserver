#include "fileio.h"

FileIO::FileIO(const QString &filename,const QIODevice::OpenModeFlag &mode)
{
    data = new QFile(filename);
    if (data->open(mode))
    {
        data_io = new QTextStream(data);
        success = true;
    }
}

FileIO::~FileIO()
{
    data->close();
    delete data;
}
