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
  // Default email parameters
  const QString emailChangePwdTitle = appName + " - 用户密码重置通知";
  inline QString emailChangePwdContent(const QString &newpwd)
  {
      return "您的密码已重置为随机密码: " + newpwd + "，请及时用该密码登陆账号并修改密码。";
  }
}
#endif
