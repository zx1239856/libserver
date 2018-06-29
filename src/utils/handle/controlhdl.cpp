#include "controlhdl.h"
#include <QTimer>

using controlhdlDetail::QTimerWrapper;

controlhdl* controlhdl::instance=nullptr;

controlhdl::controlhdl()
{

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
    mClient.insert(token, ID);
    // auto delete user after timeout
    QTimerWrapper *timer = new QTimerWrapper(token,5*60*1000);
    connect(timer,&QTimerWrapper::deleteUser,this,&controlhdl::DeleteUser);
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


QTimerWrapper::QTimerWrapper(const QString t,const int milliseconds):token(t),interval(milliseconds)
{
    timer.setTimerType(Qt::VeryCoarseTimer);
    // signal relay
    connect(&timer,&QTimer::timeout,this,[&]()
    {
        emit deleteUser(token);
    });
}

void QTimerWrapper::startTick()
{
    timer.setInterval(interval);
    timer.setSingleShot(true);
    timer.start();
}

void QTimerWrapper::stopTick()
{
    timer.stop();
}
