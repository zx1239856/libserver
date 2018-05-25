#pragma once

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
    ~config();
    QVariant Get(const QString &type);
    void Load();
    class configGarbo
    {
    public:
      ~configGarbo()
      {
        if(config::instance)delete config::instance;
      }
    };
    static configGarbo garbo;
public:
    config(const config &)=delete;
    config& operator=(config &)=delete;
    static config* getInstance();
    static void setPath(const QString &path);
    // DB configuration
    QString dbHost;
    unsigned int dbPort;
    QString dbUname;
    QString dbPwd;
    uint dbConnTimeOut;
    QString logPath;
    // superUser configuration
    QString suname;
    QString supass;
    // web
    int port;
    int ccurrency;
    // smtp
    QString smtpServer;
    int smtpPort;
    QString authType;
    QString smtpUser;
    QString smtpPwd;
    QString smtpSender;
};
