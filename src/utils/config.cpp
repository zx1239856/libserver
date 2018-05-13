#include "config.h"
#include "file/logstream.h"
#include <exception>
#include <QDebug>

config * config::instance = nullptr;
QString config::path = "";

config::config(const QString &path)
{
    settings = new QSettings(path, QSettings::NativeFormat);
    Load();
}

config::~config()
{
    delete settings;
}

config* config::getInstance()
{
  if(instance == nullptr)
    {
      instance = new config(path);
    }
  return instance;
}

void config::setPath(const QString &_path)
{
  path = _path;
}

void config::Load()
{
  try
  {
    //web
    port = Get("web/listen_port").toInt();
    ccurrency = Get("web/concurrency").toInt();
    // database
    dbHost= Get("database/host").toString();
    dbPort= Get("database/port").toInt();
    dbUname= Get("database/username").toString();
    dbPwd= Get("database/password").toString();
    //end
  }
  catch(QString &e)
  {
    *lstream << e;
    exit(EXIT_FAILURE);
  }
  catch(...)
  {
    *lstream << "Unknown error occured. Exit.";
    exit(EXIT_FAILURE);
  }
    *lstream << "All settings successfully loaded.";
}

QVariant config::Get(const QString &type)
{
    if(settings->contains(type))
        return settings->value(type);
    else
    {
        QString errMsg =  "Error: CANNOT find settings " + type;
        throw(errMsg);
    }
}
