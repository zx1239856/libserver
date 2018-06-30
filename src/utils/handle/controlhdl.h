#ifndef CONTROLHDL_H
#define CONTROLHDL_H
#include <QMap>
#include <QPair>
#include <QObject>
#include <QTimer>
#include <QThread>

namespace controlhdlDetail
{
    // A simple QThread Wrapper with eventloop enabled
    class QThreadWrapper: public QThread
    {
        Q_OBJECT
        public:
        explicit QThreadWrapper(QObject *parent= nullptr);
        void run()override;
    };


    // A simple class to wrap QTimer, because we need to store the corresponding token
    class QTimerWrapper: public QObject
    {
      Q_OBJECT
    private:
        QString token;
        QTimer *timer;
        int interval;
        QThreadWrapper *timerThread;
    public:
        QTimerWrapper(const QString token, const int milliseconds, QThreadWrapper *t);
        void startTick();
        void stopTick();
        ~QTimerWrapper();
    signals:
        void start();
        void stop();
    };
}

/// Singleton Class for control handler
class controlhdl: public QObject
{
    Q_OBJECT
    friend class QTimerWrapper;
    static controlhdl* instance;
    class controlhdlGarbo
    {
    public:
      ~controlhdlGarbo()
      {
        if(controlhdl::instance)delete controlhdl::instance;
      }
    };
    static controlhdlGarbo garbo;
    controlhdl();
    ~controlhdl();
private:
    controlhdlDetail::QThreadWrapper *timerThread;
    QMap<QString, QPair<QString, int>> mClient;  //<token, <groupName,ID> > 
    QMap<QString, controlhdlDetail::QTimerWrapper*> timers; // timer pool
public:
    // disable copy and assign
    controlhdl(const controlhdl&) = delete;
    controlhdl& operator=(controlhdl&) = delete;
    // public interfaces
    static controlhdl* getInstance();
    bool ifLogin(const QString& token);
    QPair<QString, int> GetID(const QString& token);
    void AddUser(const QString &token, const QPair<QString, int> &ID); //QPair<groupid, ID>
    void DeleteUser(const QString &token);
    void UpdateStatus(const QString &token);
};

extern controlhdl* ctrl;

#endif // HANDLE_H
