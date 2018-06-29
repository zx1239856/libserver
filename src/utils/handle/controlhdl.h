#ifndef CONTROLHDL_H
#define CONTROLHDL_H
#include <QMap>
#include <QPair>
#include <QObject>
#include <QTimer>

namespace controlhdlDetail
{
    // A simple class to wrap QTimer, because we need to store the corresponding token
    class QTimerWrapper: public QObject
    {
      Q_OBJECT
    private:
        QString token;
        QTimer timer;
        int interval;
    public:
        QTimerWrapper(const QString token, const int milliseconds);
        void startTick();
        void stopTick();
    signals:
        void deleteUser(const QString);
    };
}

class controlhdl: public QObject
{
    Q_OBJECT
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

public:
    QMap<QString, QPair<QString, int>> mClient;  //<token, <groupName,ID> >
    QMap<QString, controlhdlDetail::QTimerWrapper*> timers;
    controlhdl(const controlhdl&) = delete;
    controlhdl& operator=(controlhdl&) = delete;
    static controlhdl* getInstance();
    bool ifLogin(const QString& token);
    QPair<QString, int> GetID(const QString& token);
    void AddUser(const QString &token, const QPair<QString, int> &ID); //QPair<groupid, ID>
    void DeleteUser(const QString &token);
    void UpdateStatus(const QString &token);
};

extern controlhdl* ctrl;

#endif // HANDLE_H
