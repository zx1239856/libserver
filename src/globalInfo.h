#ifndef _GLOBALINFO_H
#define _GLOBALINFO_H

/* Modify these things to change some global constants!
*/
#include <QString>
#include <QVector>

namespace globalInfo
{
  const QString appName = "libserver";
  const QString appDescription = "Library Management Server";
  const QString appOrgDomain = "qtdaemon.serverApps";
  const QString appVersion = "0.1.0alpha";
  const QString defaultLogPath = appName+".log";
  namespace db
  {
    const QString dbName = "libserver";
    const QString dbPrefix = "lib_";
  }
  const QString dbFullPrefix = db::dbName + "." + db::dbPrefix;
  inline QString emailContent(const QString &newpwd)
  {
      return "Your temporary password is " + newpwd + ". Please change your password as soon as possible.";
  }
}
#endif
