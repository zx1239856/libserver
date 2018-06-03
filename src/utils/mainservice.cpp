#include "mainservice.h"
#include "qdaemonapplication.h"
#include "utils/crypto/token.h"
#include "qdaemonlog.h"
#include "utils/worker/bgworker.h"
#include "utils/worker/bgworkercontroller.h"
#include "utils/handle/dblog.h"
#include "utils/handle/dbsettings.h"

controlhdl* ctrl;

std::shared_ptr<bgWorkerController> mainService::pdfTasks = nullptr;
std::shared_ptr<bgWorkerController> mainService::cronTasks = nullptr;

mainService::mainService(config* _conf,QObject *parent) :
  QObject(parent),conf(_conf),server(nullptr)
{
  bgTasks.push_back(std::shared_ptr<bgWorkerController>(new bgWorkerController()));
  bgTasks.push_back(std::shared_ptr<bgWorkerController>(new bgWorkerController()));
  pdfTasks = bgTasks[0];
  cronTasks = bgTasks[1];
  bgTasks[0]->start();
  bgTasks[1]->start();
}

mainService::~mainService()
{
  if(server)
    server->deleteLater();
  bgTasks.clear();
}

void mainService::start()
{
  // dbConn Init
  dbConn::setConf(conf);
  dbConn::getInstance();
  // All init end
  //pdfConversion* conv = new pdfConversion(conf->dataDir()+"file.pdf",conf->dataDir()+"output/",pdfConversion::singleFile,200);
  //pdfTasks->addWork(conv);
  dbLog::log("logout","User logout",123,dbLog::staff);
  dbSettings::setSetting("libLogo","test",13);
  // initiate websocket
  server = new webServer;
  server->init(conf->port(),conf->ccurrency(),conf->threadKeepAliveTimeout()*1000);
  ctrl = controlhdl::getInstance();
}

void mainService::stop()
{
  // may reimplement here
  QDaemonApplication::quit();
}

std::shared_ptr<bgWorkerController> mainService::getCronController()
{
  return cronTasks;
}

std::shared_ptr<bgWorkerController> mainService::getPdfTaskController()
{
  return pdfTasks;
}
