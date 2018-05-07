// Qt lib import
#include <QCoreApplication>

#include "utils/dbwrapper/db-wrapper.h"
#include "utils/dbwrapper/db-operation.h"
#include "utils/config.h"

// global variables
static QTextStream cout(stdout, QIODevice::WriteOnly);
// global configuration file
config globalConf("./config.conf");

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
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

    return a.exec();
}
