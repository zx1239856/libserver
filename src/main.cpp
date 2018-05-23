// Qt lib import
#include <QCoreApplication>

#include "utils/mainservice.h"
#include "utils/cmdparser.h"
#include "qdaemonapplication.h"


int main(int argc, char *argv[])
{
    // argument handler
    cmdParser parser(argc,argv,"libserver");

    // Daemon instance
    QDaemonApplication app(parser.getArgc(),parser.getArgv());
    QDaemonApplication::setApplicationName("libserver example");
    QDaemonApplication::setApplicationDescription("Library Management Server");
    QDaemonApplication::setOrganizationDomain("qtdaemon.examples");
    QString confPath(parser.getConfPath());
    // mainService
    mainService service(confPath,&app);
    QObject::connect(&app,&QDaemonApplication::daemonized,&service,&mainService::start);
    QObject::connect(&app,&QDaemonApplication::aboutToQuit,&service,&mainService::stop);
    return QDaemonApplication::exec();
}
