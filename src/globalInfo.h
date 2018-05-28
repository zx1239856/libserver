#ifndef _GLOBALINFO_H
#define _GLOBALINFO_H

/* Modify these things to change some global constants!
*/
#include <QString>

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
}
#endif
