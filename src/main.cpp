// Qt lib import
#include <QCoreApplication>

#include "utils/mainservice.h"
#include "utils/cmdparser.h"
#include "qdaemonapplication.h"


int main(int argc, char *argv[])
{
    // argument handler
    cmdParser parser(argc,argv,"libserver");

    // Set configuration path
    QString confPath(parser.getConfPath());
    // Get log path individually
    // default path is
    //QFileInfo info(QCoreApplication::applicationFilePath());
    //QString logFilePath = info.absoluteDir().filePath(info.completeBaseName() + QStringLiteral(".log"));
    QString logFilePath = QStringLiteral("libserver.log");
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
    // okay, now initialize the daemon
    QDaemonApplication app(parser.getArgc(),parser.getArgv(),logFilePath);
    QDaemonApplication::setApplicationName("libserver example");
    QDaemonApplication::setApplicationDescription("Library Management Server");
    QDaemonApplication::setOrganizationDomain("qtdaemon.examples");
    // mainService
    mainService service(confPath,&app);
    QObject::connect(&app,&QDaemonApplication::daemonized,&service,&mainService::start);
    QObject::connect(&app,&QDaemonApplication::aboutToQuit,&service,&mainService::stop);
    return QDaemonApplication::exec();
}
