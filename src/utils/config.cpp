#include "config.h"
#include <exception>
#include <QDebug>

config * config::instance = nullptr;
QString config::path = "";

config::config(const QString &path)
{
    settings = new QSettings(path, QSettings::IniFormat);
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
    m_port = Get("web/listen_port").toUInt();
    m_ccurrency = Get("web/concurrency").toUInt();
    m_dataDir = Get("web/data_dir").toString();
    // logpath
    m_logPath = Get("web/log_path").toString();
    m_threadKeepAliveTimeout = Get("web/threadKeepAliveTimeout").toUInt();
    m_epoll = Get("web/useEpoll").toBool();
    // database
    m_dbHost= Get("database/host").toString();
    m_dbPort= Get("database/port").toUInt();
    m_dbUname= Get("database/username").toString();
    m_dbPwd= Get("database/password").toString();
    m_dbConnTimeOut = Get("database/connectionTimeout").toUInt();
    m_dbReadTimeOut = Get("database/readTimeout").toUInt();
    m_dbWriteTimeOut = Get("database/writeTimeout").toUInt();
    // smtp
    m_smtpServer = Get("smtp/server").toString();
    m_smtpPort = Get("smtp/port").toUInt();
    m_authType = Get("smtp/authType").toString();
    m_smtpUser = Get("smtp/username").toString();
    m_smtpPwd = Get("smtp/password").toString();
    m_smtpSender = Get("smtp/senderEmail").toString();
    m_templateDir = Get("smtp/templateDir").toString();
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
