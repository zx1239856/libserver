#ifndef MAINSERVICE_H
#define MAINSERVICE_H

#include "utils/dbwrapper/db-wrapper.h"
#include "utils/dbwrapper/db-operation.h"
#include "utils/config.h"
#include "utils/file/logstream.h"
#include "utils/web/webserver.h"
#include "utils/handle/controlhdl.h"
#include <QObject>

extern LogStream* lstream;
extern controlhdl* ctrl;

class mainService : public QObject
{
  Q_OBJECT
private:
  const QString &conf;
  webServer *server;
public:
  explicit mainService(const QString &confPath, QObject *parent = nullptr);
  void start();
  void stop();
  ~mainService();
};

#endif // MAINSERVICE_H
