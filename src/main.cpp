// Qt lib import
#include <QCoreApplication>
#include "../globalInfo.h"
#include "utils/mainservice.h"
#include "utils/cmdparser.h"
#include "qdaemonapplication.h"


int main(int argc, char *argv[])
{
    // argument handler
    cmdParser parser(argc,argv,globalInfo::appName);

    // Set configuration path
    QString confPath(parser.getConfPath());
    // Get log path individually
    // default path is
    //QFileInfo info(QCoreApplication::applicationFilePath());
    //QString logFilePath = info.absoluteDir().filePath(info.completeBaseName() + QStringLiteral(".log"));
    QString logFilePath = QStringLiteral("libserver.log");
    if(parser.isStartCommand())
      {
        QSettings *set=new QSettings(confPath,QSettings::NativeFormat);
        if(set->contains("web/log_path"))
          {
            logFilePath = set->value("web/log_path").toString();
          }
        else
          {
            printf("Error: settings file does not contain valid log_path. Use default log path instead.\n");
          }
        delete set;
        set = nullptr;
      }
    // okay, now initialize the daemon
    QDaemonApplication app(parser.getArgc(),parser.getArgv(),logFilePath);
    QDaemonApplication::setApplicationName(globalInfo::appName);
    QDaemonApplication::setApplicationDescription(globalInfo::appDescription);
    QDaemonApplication::setOrganizationDomain(globalInfo::appOrgDomain);
    QDaemonApplication::setApplicationVersion(globalInfo::appVersion);
    // mainService
    mainService service(confPath,&app);
    QObject::connect(&app,&QDaemonApplication::daemonized,&service,&mainService::start);
    QObject::connect(&app,&QDaemonApplication::aboutToQuit,&service,&mainService::stop);
    return QDaemonApplication::exec();
}
