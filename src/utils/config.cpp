#include "config.h"
#include <exception>

config::config(const QString &path)
{
  QSettings settings(path,QSettings::NativeFormat);
  dbHost=settings.value("database/host").toString();
  dbPort=settings.value("database/port").toInt();
  dbUname=settings.value("database/username").toString();
  dbPwd=settings.value("database/password").toString();
}
