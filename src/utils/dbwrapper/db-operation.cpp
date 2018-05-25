#include "db-operation.h"
#include <QTimer>

using namespace sql;

/*
 * basic SQL constructors and executors
 */

dbWrapper::Control * basicSQL::mainDBControl = nullptr;

basicSQL::basicSQL(){}

basicSQL::basicSQL(const QString &s):sql(s){}

bool basicSQL::exec()
{
  query.clear();
  query.push_back(mainDBControl->query());
  return query[0]->exec(sql);
}

void basicSQL::setControl(dbWrapper::Control *c)
{
  mainDBControl = c;
}

QSqlQuery* basicSQL::getQuery()
{
  return &(*query[0]);
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
  query.clear();
  query.push_back(mainDBControl->query());
  query[0]->prepare(sql);
  for(auto it=cont.begin();it!=cont.end();++it)
    {
      query[0]->bindValue(":"+it.key(),it.value());
    }
  return query[0]->exec();
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
  query.clear();
  query.push_back(mainDBControl->query());
  query[0]->prepare(sql);
  for(auto it=cont.begin();it!=cont.end();++it)
    {
      query[0]->bindValue(":"+it.key(),it.value());
    }
  return query[0]->exec();
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
  query.clear();
  query.push_back(mainDBControl->query());
  return query[0]->exec(sql);
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
  query.clear();
  query.push_back(mainDBControl->query());
  return query[0]->exec(sql);
}

bool select::next()
{
  return query[0]->next();
}

QVariant select::value(int i)
{
  return query[0]->value(i);
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

dbQueryThread::dbQueryThread(uint tOut, QObject *parent)
  :QThread(parent),bSql(nullptr),timeout(tOut),timeWatch(nullptr){}

void dbQueryThread::run()
{
  if(bSql == nullptr)
    {
      emit onFail(QSqlError(QString("Internal Error."),QString("Invalid query.")));
      return;
    }
  timeWatch = new QTimer;
  timeWatch->setInterval(timeout);
  timeWatch->setSingleShot(true);
  timeWatch->moveToThread(qApp->thread());
  connect(this,SIGNAL(timeOutStart()),timeWatch,SLOT(start()));
  connect(this,SIGNAL(timeOutStop()),timeWatch,SLOT(stop()));
  connect(timeWatch,&QTimer::timeout,this,[&](){
      emit onFail(QSqlError(QString("Database error."),QString("Operation timeout.")));
    });
  emit timeOutStart();
  auto ok = bSql->exec();
  emit timeOutStop();
  if(!ok)
    {
      emit onFail(bSql->getQuery()->lastError());
    }
  else
    {
      QSqlQuery *query = bSql->getQuery();
      if(query && query->isSelect())
        {
          // readEverything here
          // and emit onResult signal
          // after finish
          QVector<QSqlRecord> result;
          for(int i=0;i<query->size(); ++i)
          {
             query->seek(i);
             result.push_back(query->record());
          }
          emit onResult(result);
        }
      else
        {
           emit onSuccess();
        }
    }
}

dbQueryThread::~dbQueryThread()
{
  if(timeWatch)
  {
      delete timeWatch;
      timeWatch->moveToThread(QThread::currentThread());
  }
}

void dbQueryThread::setSqlQuery(sql::basicSQL *_sql)
{
  bSql = _sql;
}

// dbQueryThread end
