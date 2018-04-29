#pragma once

#include <QtCore>
#include <QSettings>
/*
 * Read basic configuration file and parse parameters
 *
 * Usage: config conf(PATH);
 */

struct config
{
  // DB configuration
  QString dbHost;
  unsigned int dbPort;
  QString dbUname;
  QString dbPwd;
  // network configuration
  unsigned int listenPort;

  // superUser configuration
  QString suname;
  QString supass;

  config(const QString &path);
private:

};
