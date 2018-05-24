#include "mainservice.h"
#include "qdaemonapplication.h"

controlhdl* ctrl;

mainService::mainService(const QString& confPath,QObject *parent) :
  QObject(parent),conf(confPath),server(nullptr)
{}

mainService::~mainService()
{
  if(server)
    server->deleteLater();
}

void mainService::start()
{
  // Configuration File Init
  config::setPath(conf);
  config *conf = config::getInstance();
  // dbConn Init
  dbConn::setConf(conf);
  dbConn::getInstance();
  // All init end

  // other miscellaneous stuffs here
  // this db part here would cause The acquired DBus interface replied erroneously.
  // for unknown reasons
  /*sql::select *sel= new sql::select("libserver.lib_books");
  dbQueryThread *dbT= new dbQueryThread(sel,conf->dbConnTimeOut);
  QObject::connect(dbT,&dbQueryThread::onResult,this,
                   [&](const QVector<QSqlRecord>& res)
  {
      dbT->quit();
      for(int i=0;i<res.size();++i)
        {
          for(int j=0;j<res[i].count();++j)
            {
              qWarning() << res[i].value(j);
            }
        }
      dbT->deleteLater();
  },Qt::DirectConnection);
  QObject::connect(dbT,&dbQueryThread::onFail,this,
                   [&](const QSqlError &err)
  {
      qWarning() << err;
      dbT->terminate();
      dbT->wait();
      dbT->deleteLater();
    },Qt::DirectConnection);
  dbT->start();*/

  // initiate websocket
  server = new webServer;
  server->init(conf->port,conf->ccurrency);

  ctrl = controlhdl::getInstance();
}

void mainService::stop()
{
  // may reimplement here
  QDaemonApplication::quit();
}
