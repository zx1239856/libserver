#include "controlhdl.h"
#include <QTimer>
#include <QDebug>
#include <QThread>
#include <QCoreApplication>
using namespace controlhdlDetail;

controlhdl::controlhdlGarbo controlhdl::garbo;

QThreadWrapper::QThreadWrapper(QObject *parent):QThread(parent){}

void QThreadWrapper::run()
{
    // open event loop here
    exec();
}

controlhdl* controlhdl::instance=nullptr;

controlhdl::controlhdl():timerThread(new QThreadWrapper())
{
}

controlhdl::~controlhdl()
{
    timerThread->exit();
    timerThread->wait();
    if(timerThread)
      {
        delete timerThread;
        timerThread = nullptr;
      }
}

bool controlhdl::ifLogin(const QString& token)
{
    return mClient.contains(token);
}

QPair<QString, int> controlhdl::GetID(const QString& token)
{
    return mClient[token];
}

void controlhdl::AddUser(const QString &token, const QPair<QString, int> &ID)
{
    if(!timerThread->isRunning())
      {
         timerThread->start();
      }
    mClient.insert(token, ID);
    // auto delete user after timeout
    QTimerWrapper *timer = new QTimerWrapper(token,5*60*1000,timerThread);
    // **** Change Timeout here **** normally it is set to 5 minutes
    // if user dont't present any operation after 5 mins, the token would expire automatically
    timer->startTick();
    timers[token] = timer;
}

void controlhdl::DeleteUser(const QString &token)
{
    mClient.erase(mClient.find(token));
    auto it =timers.find(token);
    if(it!=timers.end())
    {
        it.value()->deleteLater();
        timers.erase(it);
    }
}

controlhdl* controlhdl::getInstance()
{
    if(instance==nullptr)
    {
        instance = new controlhdl();
    }
    return instance;
}

void controlhdl::UpdateStatus(const QString &token)
{
    auto it = timers.find(token);
    if(it!=timers.end())
    {
        it.value()->stopTick();
        it.value()->startTick();
    }
}


QTimerWrapper::QTimerWrapper(const QString t,const int milliseconds,QThreadWrapper *thread):token(t),timer(new QTimer),interval(milliseconds),timerThread(thread)
{
    // signal relay
    connect(timer,&QTimer::timeout,this,[&]()
    {
        ctrl->DeleteUser(token);
    },Qt::DirectConnection);
    connect(this,SIGNAL(start()),timer,SLOT(start()));
    connect(this,&QTimerWrapper::stop,timer,&QTimer::stop);
    timer->setTimerType(Qt::CoarseTimer);
    timer->setInterval(interval);
    timer->setSingleShot(true);
    timer->moveToThread(timerThread);
}

void QTimerWrapper::startTick()
{
    emit start();
}

void QTimerWrapper::stopTick()
{
    emit stop();
}

QTimerWrapper::~QTimerWrapper()
{
  if(timer)
    {
      timer->deleteLater();
    }
}
