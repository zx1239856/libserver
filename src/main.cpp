// Qt lib import
#include <QCoreApplication>

#include "utils/dbwrapper/db-wrapper.h"
#include "utils/dbwrapper/db-operation.h"
#include "utils/config.h"
#include "utils/file/logstream.h"
#include "utils/web/webserver.h"
#include "utils/handle/controlhdl.h"

LogStream* lstream;
controlhdl* ctrl;

void usage(char *argv[])
{
  qWarning() << "";
  qWarning() << argv[0] << " start [conf-path]";
  qWarning() << argv[0] << " stop";
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QString confPath;
    // argument handlers
    if(argc==1)
      {
        usage(argv);
        exit(EXIT_FAILURE);
      }
    else if(argc == 2)
      {
        if(!strcmp(argv[1],"stop"))exit(EXIT_SUCCESS);
        else
          {
            usage(argv);
            exit(EXIT_FAILURE);
          }
      }
    else if(argc == 3)
      {
        if(!strcmp(argv[1],"start"))
          {
            confPath = QString(argv[2]);
          }
        else
          {
            usage(argv);
            exit(EXIT_FAILURE);
          }
      }
    else
      {
        usage(argv);
        exit(EXIT_FAILURE);
      }
    // argument handlers end

    // Configuration File Init
    config::setPath(confPath);
    config *conf = config::getInstance();

    //Initiate LogStream
    LogStream::setPath(conf->logPath);
    lstream = LogStream::getInstance();

    // dbConn Init
    dbConn::setConf(conf);
    // All init end

    // other miscellaneous stuffs here
    sql::select sel("lib_user.user");
    dbQueryThread dbT(&sel);
    QObject::connect(&dbT,&dbQueryThread::onResult,
                     [&](const QVector<QSqlRecord>& res)
    {
        for(int i=0;i<res.size();++i)
          {
            for(int j=0;j<res[i].count();++j)
              {
                qWarning() << res[i].value(j);
              }
          }
    });
    dbT.start();
    /*QThreadPool::globalInstance()->setMaxThreadCount(10);

    for Loop indicates how many threads
    and put the function into QtConcurrent::run() to run
    }*/


    // initiate websocket
    webServer server;
    server.init(conf->port,conf->ccurrency);

    ctrl = controlhdl::getInstance();
    // server.init(conf.port, conf.ccurrency);
    return a.exec();
}
