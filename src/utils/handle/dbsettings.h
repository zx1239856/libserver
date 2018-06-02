#ifndef DBSETTINGS_H
#define DBSETTINGS_H

class QString;

class dbSettings
{
public:
  dbSettings();
  static QString getSetting(const QString valName);
  static bool setSetting(const QString valName,const QString value, unsigned int opID);
};

#endif // DBSETTINGS_H
