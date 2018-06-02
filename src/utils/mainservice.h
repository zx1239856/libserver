#ifndef MAINSERVICE_H
#define MAINSERVICE_H

#include "utils/dbwrapper/db-wrapper.h"
#include "utils/dbwrapper/db-operation.h"
#include "utils/config.h"
#include "utils/web/webserver.h"
#include "utils/handle/controlhdl.h"
#include "utils/worker/bgworkercontroller.h"
#include <QObject>

extern controlhdl* ctrl;

class mainService : public QObject
{
  Q_OBJECT
private:
  config *conf;
  webServer *server;
  std::vector<std::shared_ptr<bgWorkerController>> bgTasks;
  static std::shared_ptr<bgWorkerController> pdfTasks;
  static std::shared_ptr<bgWorkerController> cronTasks;
public:
  explicit mainService(config *conf, QObject *parent = nullptr);
  std::shared_ptr<bgWorkerController> getPdfTaskController();
  std::shared_ptr<bgWorkerController> getCronController();
  void start();
  void stop();
  ~mainService();
};

#endif // MAINSERVICE_H
