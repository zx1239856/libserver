#pragma once

#include "utils/file/logstream.h"
#include <QtCore>
#include <QSettings>

/*
 * Read basic configuration file and parse parameters
 *
 * Usage: config conf(PATH);
 */

struct config
{
    QSettings* settings;

public:
    config(const QString &path);
    ~config();
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
