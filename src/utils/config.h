#pragma once

#include <QtCore>
#include <QSettings>

/*
 * Read basic configuration file and parse parameters
 *
 * Usage: config conf(PATH);
 */

// PropertyDeclare Start
// Useful Macro to declare private Var and read it
#define Property(Type, Name, ...)                   \
    private:                                                        \
    Type m_ ## Name __VA_ARGS__;                                    \
    public:                                                         \
    inline const Type &Name(void) const { return m_ ## Name; }      \
    private:
// PropertyDeclare End

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
 /* Specific data here */
    // DB configuration
    Property(QString,dbHost)
    Property(unsigned int,dbPort)
    Property(QString, dbUname)
    Property(QString, dbPwd)
    Property(uint, dbConnTimeOut)
    Property(uint,dbReadTimeOut)
    Property(uint,dbWriteTimeOut)
    /*// superUser configuration
    QString suname;
    QString supass;*/
    // web
    Property(uint, port)
    Property(QString,logPath)
    Property(uint,ccurrency)
    Property(QString,dataDir)
    Property(uint,threadKeepAliveTimeout)
    Property(bool,epoll)
    // smtp
    Property(QString,smtpServer)
    Property(uint,smtpPort)
    Property(QString,authType)
    Property(QString,smtpUser)
    Property(QString,smtpPwd)
    Property(QString,smtpSender)
};
