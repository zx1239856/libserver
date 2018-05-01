#include "db-operation.h"

using namespace sql;

extern config globalConf;

// singleton initialization
config * dbConn::conf = &globalConf;
dbConn * dbConn::instance = new dbConn();

dbWrapper::Control* basicSQL::mainDBControl=dbConn::getInstance()->getControl();
// singleton init end

/*
 * SQL constructors and executors
 */

basicSQL::basicSQL():
  query(mainDBControl->query()){}

basicSQL::basicSQL(const QString &s):
  query(mainDBControl->query()),sql(s){}

bool basicSQL::exec()
{
  return query->exec(sql);
}

QSqlError basicSQL::lastError()
{
  return query->lastError();
}

insert::insert(const QString &_tableName,
               const QMap<QString,QVariantList> &_content):
  basicSQL(),
  tablename(_tableName),content(_content)
{
  // construct SQL sentence
  sql="INSERT INTO "+_tableName +" (";  // INSERT INTO tablename (a,b,c) VALUES (?, ? ,?)
  for(auto it=content.keys().begin();it!=content.keys().end();++it)
    {
      sql += *it;
      if((it+1)!=content.keys().end())sql+=",";
    }
  sql+=") VALUES (";
  for(auto it=content.keys().begin();it!=content.keys().end();++it)
    {
      sql.append(':');
      sql += *it;
      if((it+1)!=content.keys().end())sql+=",";
    }
  sql+=")";
}

bool insert::exec()
{
  query->prepare(sql);
  for(int i=0;i<content.keys().size();++i)
    {
      query->bindValue(":"+content.keys()[i],content.values()[i]);
    }
  return query->execBatch();
}

update::update(const QString& _tablename,
               QMap<QString,QVariant>& _content,
               const QString& _filter):
  basicSQL(),
  tablename(_tablename),content(_content),filter(_filter)
{
  sql="UPDATE "+tablename+" SET ";
  for(auto it=content.begin();it!=content.end();++it)
    {
      sql.append(it.key()+"=:");
      sql.append(it.key());
      if((it+1)!=content.end())sql.append(",");
    }
  sql+=" WHERE ";
  sql+=filter;
}

bool update::exec()
{
  query->prepare(sql);
  for(auto it=content.begin();it!=content.end();++it)
    {
      query->bindValue(":"+it.key(),it.value());
    }
  return query->exec();
}

del::del(const QString &_tablename,const QString &_filter):
  basicSQL(),
  tablename(_tablename),filter(_filter)
{
  sql="DELETE FROM "+tablename +" WHERE ";
  sql+=filter;
}

bool del::exec()
{
  return query->exec(sql);
}

select::select(const QString& _tablename,const QString _filter):
  basicSQL(),
  tablename(_tablename),filter(_filter)
{
  sql="SELECT * FROM "+tablename+" WHERE "+filter;
}

select::select(const QVector<QString> &_what,const QString& _tablename,const QString _filter):
  basicSQL(),
  what(_what),tablename(_tablename),filter(_filter)
{
  sql="SELECT ";
  for(auto it=what.begin();it!=what.end();++it)
    {
      sql+=(*it);
      if((it+1)!=what.end())sql+=",";
    }
  sql+=" FROM " + tablename+" WHERE ";
  sql+=filter;
}

bool select::exec()
{
  return query->exec(sql);
}

bool select::next()
{
  return query->next();
}

QVariant select::value(int i)
{
  return query->value(i);
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
