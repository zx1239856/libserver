#include "db-operation.h"

using namespace sql;

extern config globalConf;

// singleton initialization
config * dbConn::conf = &globalConf;
dbConn * dbConn::instance = new dbConn();

dbWrapper::Control* basicSQL::mainDBControl=dbConn::getInstance()->getControl();
// singleton init end

/*
 * SQL constructors and excutors
 */
insert::insert(const QString &_tableName,
       const std::vector<QString> &_fields,
       const std::vector<QVariantList> &_vals):
  basicSQL(),
  tablename(_tableName),fields(_fields),vals(_vals)
{
  // construct SQL sentence
  sql="INSERT INTO "+_tableName +" (";  // INSERT INTO tablename (a,b,c) VALUES (?, ? ,?)
  for(auto it=fields.begin();it!=fields.end();++it)
    {
      sql += *it;
      if((it+1)!=fields.end())sql+=",";
    }
  sql+=") VALUES (";
  for(auto it=fields.begin();it!=fields.end();++it)
    {
      sql += ":"+(*it);
      if((it+1)!=fields.end())sql+=",";
    }
  sql+=")";
}

void insert::exec()
{
  auto query(mainDBControl->query());
  query->prepare(sql);
  for(uint i=0;i<fields.size();++i)
    {
      query->bindValue(":"+fields[i],vals[i]);
    }
  query->execBatch();
}

update::update(const QString &_table,const QString &_filter):
  tablename(_table),filter(_filter)
{}

void update::exec()
{

}

// dbConn class
// use singleton mode
dbConn::dbConn(unsigned int threads, const QString &dbName):
  control(new dbWrapper::Control({"QMYSQL","DBConn",conf->dbHost,dbName,conf->dbUname,conf->dbPwd})),
  threadCount(threads)
{}

dbConn::~dbConn()
{
  if(control)delete control;
}

dbConn* dbConn::getInstance()
{
  return instance;
}

dbWrapper::Control* dbConn::getControl()
{
  return control;
}
