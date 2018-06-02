#ifndef BGWORKERCONTROLLER_H
#define BGWORKERCONTROLLER_H

#include <QThread>
#include <map>
#include <set>
#include "bgworker.h"
#include "qcron.h"

namespace bgControllerPriv {
  class process:public QObject
  {
    Q_OBJECT
  public:
    explicit process(QObject *parent=nullptr);
    void processSingle(AbstractWorker *work);
    void processMulti(AbstractWorker *work);
  signals:
    void singleFinish(AbstractWorker *work);
  };
}

class bgWorkerController:public QThread
{
  Q_OBJECT
  QThread *managedThread = nullptr;
  std::set<AbstractWorker*> singleWork;
  std::map<QCron*,AbstractWorker*> multiWork;
  bgControllerPriv::process *proc;
public:
  explicit bgWorkerController(QObject *parent = nullptr);
  // for works run only once
  void addWork(AbstractWorker *newWork);
  // for works run multiple times
  void addWork(AbstractWorker *newWork, const QString cronPattern);
  void run()override;
  ~bgWorkerController();
signals:
  void singleWorkAdded(AbstractWorker *work);
};

#endif // BGWORKERCONTROLLER_H
