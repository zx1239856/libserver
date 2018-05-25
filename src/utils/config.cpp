#include "config.h"
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
    // logpath
    logPath = Get("web/log_path").toString();
    //web
    port = Get("web/listen_port").toInt();
    ccurrency = Get("web/concurrency").toInt();
    // database
    dbHost= Get("database/host").toString();
    dbPort= Get("database/port").toInt();
    dbUname= Get("database/username").toString();
    dbPwd= Get("database/password").toString();
    dbConnTimeOut = Get("database/connectionTimeout").toUInt();
    //end
  }
  catch(std::string &e)
  {
    printf("%s\n",e.c_str());
    exit(EXIT_FAILURE);
  }
  catch(...)
  {
    printf("Unknown error occured. Exit.\n");
    exit(EXIT_FAILURE);
  }
#ifdef VERBOSE_OUTPUT
    printf("All settings successfully loaded.\n");
#endif
}

QVariant config::Get(const QString &type)
{
    if(settings->contains(type))
        return settings->value(type);
    else
    {
        std::string errMsg =  "Error: CANNOT find settings " + type.toStdString();
        throw(errMsg);
    }
}
