// Qt lib import
#include <QCoreApplication>

#include "utils/dbwrapper/db-wrapper.h"
#include "utils/dbwrapper/db-operation.h"
#include "utils/config.h"
#include "utils/file/logstream.h"
#include "utils/web/webserver.h"

// global variables
static QTextStream cout(stdout, QIODevice::WriteOnly);

LogStream* lstream;

// global configuration file
config globalConf("./config.conf");
globalConf->load();

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
	
	// load logstream
    lstream = new LogStream();
	
    QMap<QString,QVariant> map;
    map.insert(map.end(),"typename","user");
    map.insert(map.end(),"privilege",true);
    map.insert(map.end(),"id",1);
    sql::insert up("lib_settings.stafftype",map);
    up.exec();
    sql::select sel("lib_settings.stafftype","id",sql::COUNT);
    if(!sel.exec())
      {
        qWarning()<<sel.lastError();
      }
    while(sel.next())
      {
       auto res = sel.value(0);
       qWarning()<<res;
      }
    /*QThreadPool::globalInstance()->setMaxThreadCount(10);

    for Loop indicates how many threads
    and put the function into QtConcurrent::run() to run
    }*/

    // initiate websocket
    webServer server;
    server.init(8080, 5);
    // server.init(conf.port, conf.ccurrency);
    return a.exec();
}
