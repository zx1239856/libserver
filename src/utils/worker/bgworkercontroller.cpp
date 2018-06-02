#include "bgworkercontroller.h"
#include <QDebug>

using namespace bgControllerPriv;

process::process(QObject *parent):QObject(parent){}

void process::processSingle(AbstractWorker *work)
{
  QObject::connect(work,&AbstractWorker::onFail,this,[&](const QString &err)
  {
      qDebug() << err;
    });
  QObject::connect(work,&AbstractWorker::onFinish,this,[&]()
  {
      emit singleFinish(work);
      return;
    });
  work->run();
}

void process::processMulti(AbstractWorker *work)
{
  QObject::connect(work,&AbstractWorker::onFail,this,[&](const QString &err)
  {
      qDebug() << err;
    });
  work->run();
}

bgWorkerController::bgWorkerController(QObject *parent):QThread(parent),proc(nullptr)
{}


void bgWorkerController::addWork(AbstractWorker *newWork)
{
  while(true)
    {
      if(managedThread)
        {
          newWork->moveToThread(managedThread);
          break;
        }
      else
        {
          qDebug() << "Warning, controller thread not started. Wait until it start";
        }
    }
  singleWork.insert(newWork);
  emit singleWorkAdded(newWork);
}

void bgWorkerController::addWork(AbstractWorker *newWork, const QString cronPattern)
{
  while(true)
    {
      if(managedThread)
        {
          newWork->moveToThread(managedThread);
          break;
        }
      else
        {
          qDebug() << "Warning, controller thread not started. Wait until it start";
        }
    }
  auto cron = new QCron(cronPattern);
  multiWork.insert({cron,newWork});
  // the work is processed when the cron is active
  // no need to destroy after finish
  connect(cron,&QCron::activated,proc,[&](QCron *c)
  {
    auto it = multiWork.find(c);
    if(it!=multiWork.end())
      {
        proc->processMulti(it->second);
      }
  },Qt::QueuedConnection);
}

void bgWorkerController::run()
{
  managedThread = currentThread();
  proc = new process();
  connect(this,&bgWorkerController::singleWorkAdded,proc,&process::processSingle,Qt::QueuedConnection);
  connect(proc,&process::singleFinish,this,[&](AbstractWorker *work)
  {
    auto it = singleWork.find(work);
    if(it!=singleWork.end())
      {
        singleWork.erase(it);
        (*it)->deleteLater();
      }
  });
  exec();
}

bgWorkerController::~bgWorkerController()
{
  for(auto it = multiWork.begin();it!=multiWork.end();++it)
    {
      if(it->second)delete it->second;
      if(it->first)it->first->deleteLater();
    }
  if(proc)delete proc;
}
