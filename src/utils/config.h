#pragma once

#include "utils/file/logstream.h"
#include <QtCore>
#include <QSettings>

/*
 * Read basic configuration file and parse parameters
 *
 * Usage: config conf(PATH);
 */

class config
{
    QSettings* settings;
    static config *instance;
    static QString path;
    config(const QString &path);
    class configGarbo
    {
    public:
      ~configGarbo()
      {
        if(config::instance)delete config::instance;
      }
    };
public:
    ~config();
    static config* getInstance();
    static void setPath(const QString &path);
    QVariant Get(const QString &type);
    void Load();
    // DB configuration
    QString dbHost;
    unsigned int dbPort;
    QString dbUname;
    QString dbPwd;
    // superUser configuration
    QString suname;
    QString supass;
    //web
    int port;
    int ccurrency;
};
