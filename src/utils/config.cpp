#include "config.h"
#include <exception>

config::config(const QString &path)
{
    settings = new QSettings(path, QSettings::NativeFormat);
    Load();
}

config::~config()
{
    delete settings;
}

void config::Load()
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
    *lstream << "All settings loaded.";
}

QVariant config::Get(const QString &type)
{
    if(settings->contains(type))
        return settings->value(type);
    else
    {
        *lstream << "Error: CANNOT find settings " + type;
        QVariant null;
        return null;
    }
}
