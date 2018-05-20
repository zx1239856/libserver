#include "db-operation.h"

using namespace sql;

/*
 * basic SQL constructors and executors
 */

dbWrapper::Control * basicSQL::mainDBControl = nullptr;

basicSQL::basicSQL():query(mainDBControl->query()){}

basicSQL::basicSQL(const QString &s):query(mainDBControl->query()),sql(s){}

bool basicSQL::exec()
{
  return query->exec(sql);
}

QSqlError basicSQL::lastError()
{
  return query->lastError();
}

void basicSQL::setControl(dbWrapper::Control *c)
{
  mainDBControl = c;
}

bool basicSQL::isSelect()
{
  return query->isSelect();
}

QSqlQuery* basicSQL::getQuery()
{
  return &(*query);
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
  return query->exec();
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
  return query->exec();
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
  return query->exec(sql);
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

config * dbConn::conf = nullptr;
dbConn * dbConn::instance = nullptr;
unsigned int dbConn::threadCount = 4;

dbConn::dbConn(const QString &dbName):
  control(new dbWrapper::Control({"QMYSQL","DBConn",conf->dbHost,dbName,conf->dbUname,conf->dbPwd}))
{
  sql::basicSQL::setControl(control);
}

dbConn::~dbConn()
{
  if(control)delete control;
}

void dbConn::setConf(config *_conf)
{
  conf = _conf;
  threadCount = 4; // this is a default val of MaxThreads
  dbConn::getInstance();
}

void dbConn::setMaxThread(unsigned int t)
{
  threadCount = t;
}

dbConn* dbConn::getInstance()
{
  if(instance == nullptr)
    {
      instance= new dbConn();
    }
  return instance;
}

dbWrapper::Control* dbConn::getControl()
{
  return control;
}
// dbConn end

// dbQueryThread start

dbQueryThread::dbQueryThread(basicSQL *sql,QObject *parent)
  :QThread(parent),bSql(sql){}

void dbQueryThread::run()
{
  if(!bSql->exec())
    {
      emit onFail(bSql->lastError());
    }
  else
    {
      if(bSql->isSelect())
        {
          sql::select* pSelect = dynamic_cast<sql::select*>(bSql);
          if(pSelect!=nullptr)
            {
              // readEverything here
              // and emit onResult signal
              // after finish
              QVector<QSqlRecord> result;
              QSqlQuery *q=pSelect->getQuery();
              for(int i=0;i<q->size();++i)
                {
                  q->seek(i);
                  result.push_back(q->record());
                }
              emit onResult(result);
            }
        }
      else
        {
           emit onSuccess();
        }
    }
}

dbQueryThread::~dbQueryThread(){}

// dbQueryThread end
