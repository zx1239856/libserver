// Qt lib import
#include <QCoreApplication>
#include "../globalInfo.h"
#include "utils/mainservice.h"
#include "utils/cmdparser.h"
#include "qdaemonapplication.h"
#include "epoll/eventdispatcher_epoll.h"

int main(int argc, char *argv[])
{
    // argument handler
    cmdParser parser(argc,argv,globalInfo::appName);

    // Set configuration path
    QString confPath(parser.getConfPath());

    config::setPath(confPath);
    // default log path
    QString logFilePath = "libserver.log";
    config *conf= nullptr;
    if(parser.isStartCommand())
      {
        // Configuration File Init
        // First check to ensure its validity
        conf = config::getInstance();
        if(!conf->logPath().isEmpty())
          {
            logFilePath = conf->logPath();
          }
        else
          {
            printf("Warning: Logfile path was not set properly, use default logPath instead.\n");
          }
        // EventPool works if and only if for linux
        if(conf->epoll())
          {
#ifdef Q_OS_LINUX
    QCoreApplication::setEventDispatcher(new EventDispatcherEPoll);
#endif
          }
      }
    // okay, now initialize the daemon
    QDaemonApplication app(parser.getArgc(),parser.getArgv(),logFilePath);
    QDaemonApplication::setApplicationName(globalInfo::appName);
    QDaemonApplication::setApplicationDescription(globalInfo::appDescription);
    QDaemonApplication::setOrganizationDomain(globalInfo::appOrgDomain);
    QDaemonApplication::setApplicationVersion(globalInfo::appVersion);
    // mainService
    mainService service(conf,&app);
    QObject::connect(&app,&QDaemonApplication::daemonized,&service,&mainService::start);
    QObject::connect(&app,&QDaemonApplication::aboutToQuit,&service,&mainService::stop);
    return QDaemonApplication::exec();
}
