#include "db-operation.h"

using namespace sql;

extern config globalConf;

// singleton initialization
config * dbConn::conf = &globalConf;
dbConn * dbConn::instance = new dbConn();

dbWrapper::Control* basicSQL::mainDBControl=dbConn::getInstance()->getControl();
// singleton init end

/*
 * basic SQL constructors and executors
 */

basicSQL::basicSQL():
  query(mainDBControl->query()){}

basicSQL::basicSQL(const QString &s):
  query(mainDBControl->query()),sql(s){}

bool basicSQL::exec()
{
  bool stat = query->exec(sql);
  if(stat)emit onSuccess();
  else emit onFail(query->lastError());
  return stat;
}

QSqlError basicSQL::lastError()
{
  return query->lastError();
}

/*
 * insert
 */

insert::insert(const QString &tableName,
               const QMap<QString,QVariant> &content):
  basicSQL(),cont(content)
{
  // construct SQL sentence
  sql="INSERT INTO "+tableName +" (";  // INSERT INTO tablename (col1,col2,col3) VALUES (?, ? ,?)
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
  for(auto it=cont.begin();it!=cont.end();++it)
    {
      query->bindValue(":"+it.key(),it.value());
    }
  bool stat = query->exec();
  if(stat)emit onSuccess();
  else emit onFail(query->lastError());
  return stat;
}

/* insert end */

/* update */
update::update(const QString& tablename,
               QMap<QString,QVariant>& content,
               const QString& condition):
  basicSQL(),
  cont(content)
{
  sql="UPDATE "+tablename+" SET ";
  for(auto it=content.begin();it!=content.end();++it)
    {
      sql.append(it.key()+"=:");
      sql.append(it.key());
      if((it+1)!=content.end())sql.append(",");
    }
  sql+=" WHERE ";
  sql+=condition;
}

bool update::exec()
{
  query->prepare(sql);
  for(auto it=cont.begin();it!=cont.end();++it)
    {
      query->bindValue(":"+it.key(),it.value());
    }
  bool stat = query->exec();
  if(stat)emit onSuccess();
  else emit onFail(query->lastError());
  return stat;
}
/* update end */

del::del(const QString &tablename,const QString &condition):
  basicSQL()
{
  sql="DELETE FROM "+tablename +" WHERE ";
  sql+=condition;
}

bool del::exec()
{
  bool stat = query->exec(sql);
  if(stat)emit onSuccess();
  else emit onFail(query->lastError());
  return stat;
}

/* delete end */

/* select */

select::select(const QString& tablename,const QString &condition,const QList<QString> &what,bool isDistinct):
  basicSQL()
{
  sql="SELECT ";
  if(isDistinct)sql+="DISTINCT ";
  for(auto it=what.begin();it!=what.end();++it)
    {
      sql+=(*it);
      if((it+1)!=what.end())sql+=",";
    }
  sql+=" FROM " + tablename+" WHERE ";
  sql+=condition;
}

select::select(const QString& tablename,const QList<QString> &what,bool isDistinct):
  select(tablename,"1",what,isDistinct){}

select::select(const QString &tablename,const QString &col, func sqlfunc):
  basicSQL()
{
  sql="SELECT ";
  switch(sqlfunc)
    {
    case MIN:sql+="MIN";break;
    case MAX:sql+="MAX";break;
    case AVG:sql+="AVG";break;
    case SUM:sql+="SUM";break;
    case COUNT:sql+="COUNT";break;
    }
  sql+=("("+col+")");
  sql+=(" FROM "+tablename);
}

void select::addLimit(uint limit,uint start_pos)
{
  if(!isLimit)
    {
      sql+=(" LIMIT "+QString::number(start_pos)+","+QString::number(limit));
      isLimit=true;
    }
}

void select::addOrder(const QString& colname, order sqlorder)
{
  if(!isOrder)
    {
      sql+=(" ORDER BY "+colname);
      switch(sqlorder)
        {
        case DESC:sql+=" DESC";break;
        case ASC:sql+=" ASC";break;
        }
      isOrder=true;
    }
}

void select::addOrder(const QList<QString>& cols,order sqlorder)
{
  if(!isOrder)
    {
      sql+=" ORDER BY ";
      for(auto it=cols.begin();it!=cols.end();++it)
        {
          sql+=*it;
          if((it+1)!=cols.end())sql+=",";
        }
      switch(sqlorder)
        {
        case DESC:sql+=" DESC";break;
        case ASC:sql+=" ASC";break;
        }
      isOrder=true;
    }
}

bool select::exec()
{
  bool stat = query->exec(sql);
  if(stat)emit onSuccess();
  else emit onFail(query->lastError());
  return stat;
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
// dbConn end

// start dbQueryQueue

dbQueryQueue::dbQueryQueue(uint maxT):maxThread(maxT){}

void dbQueryQueue::addQuery(sql::basicSQL* query)
{
  QueryQueue.enqueue(query);
}

void dbQueryQueue::run()
{

}



