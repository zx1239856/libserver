#include "db-operation.h"
#include <QTimer>

using namespace sql;

/*
 * basic SQL constructors and executors
 */

dbWrapper::Control * basicSQL::mainDBControl = nullptr;
dbConn::dbConnGarbo dbConn::Garbo;

basicSQL::basicSQL():query(mainDBControl->query())
{
  connect(this,&basicSQL::onFail,this,[&](const QSqlError& e){this->err=e;});
}

basicSQL::basicSQL(const QString &s):query(mainDBControl->query()),sql(s)
{
  connect(this,&basicSQL::onFail,this,[&](const QSqlError& e){this->err=e;});
}

QSqlError basicSQL::lastError()
{
  return err;
}

bool basicSQL::exec()
{
  if(!query.getDataBase()->isOpen())
    {
      emit onFail(query.getDataBase()->lastError());
      return false;
    }
  if(!query->exec(sql))
    {
      emit onFail(query->lastError());
      return false;
    }
  else
    {
      if(query->isSelect())
        {
          emit onResult(this->toResult());
        }
      else emit onSuccess();
      return true;
    }
}

void basicSQL::setControl(dbWrapper::Control *c)
{
  mainDBControl = c;
}

QVector<QSqlRecord> basicSQL::toResult()
{
  QVector<QSqlRecord> result;
  for(int i=0;i<query->size(); ++i)
  {
     query->seek(i);
     result.push_back(query->record());
  }
  return result;
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
  for(int it=0;it<content.keys().size();++it)
    {
      sql.append(content.keys()[it]);
      if((it+1)<content.keys().size())sql.append(',');
    }
  sql+=") VALUES (";
  for(int it=0;it<content.keys().size();++it)
    {
      sql.append(':');
      sql.append(content.keys()[it]);
      if((it+1)<content.keys().size())sql.append(',');
    }
  sql+=")";
}

bool insert::exec()
{
  if(!query.getDataBase()->isOpen())
    {
      emit onFail(query.getDataBase()->lastError());
      return false;
    }
  query->prepare(sql);
  for(auto it=cont.begin();it!=cont.end();++it)
    {
      query->bindValue(":"+it.key(),it.value());
    }
  if(!query->exec())
    {
      emit onFail(query->lastError());
      return false;
    }
  else
    {
      emit onSuccess();
      return true;
    }
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
  if(!query.getDataBase()->isOpen())
    {
      emit onFail(query.getDataBase()->lastError());
      return false;
    }
  query->prepare(sql);
  for(auto it=cont.begin();it!=cont.end();++it)
    {
      query->bindValue(":"+it.key(),it.value());
    }
  if(!query->exec())
    {
      emit onFail(query->lastError());
      return false;
    }
  else
    {
      emit onSuccess();
      return true;
    }
}
/* update end */

del::del(const QString &tablename,const QString &condition):
  basicSQL()
{
  sql="DELETE FROM "+tablename +" WHERE ";
  sql+=condition;
}

/* delete end */

/* select */

select::select(const QString& tablename,const QString &condition,const QList<QString> &what,bool isDistinct):
  basicSQL()
{
  sql="SELECT ";
  if(isDistinct)sql.append("DISTINCT ");
  for(auto it=what.begin();it!=what.end();++it)
    {
      sql.append(*it);
      if((it+1)!=what.end())sql.append(',');
    }
  sql.append(" FROM " + tablename+" WHERE ");
  sql.append(condition);
}

select::select(const QString& tablename,const QList<QString> &what,bool isDistinct):
  select(tablename,"1",what,isDistinct){}

select::select(const QString &tablename,const QString &col, func sqlfunc):
  basicSQL()
{
  sql="SELECT ";
  switch(sqlfunc)
    {
    case MIN:sql.append("MIN");break;
    case MAX:sql.append("MAX");break;
    case AVG:sql.append("AVG");break;
    case SUM:sql.append("SUM");break;
    case COUNT:sql.append("COUNT");break;
    }
  sql.append("("+col+")");
  sql.append(" FROM "+tablename);
}

void select::addLimit(uint limit,uint start_pos)
{
  if(!isLimit)
    {
      sql.append(" LIMIT "+QString::number(start_pos)+","+QString::number(limit));
      isLimit=true;
    }
}

void select::addOrder(const QString& colname, order sqlorder)
{
  if(!isOrder)
    {
      sql.append(" ORDER BY "+colname);
      switch(sqlorder)
        {
        case DESC:sql.append(" DESC");break;
        case ASC:sql.append(" ASC");break;
        }
      isOrder=true;
    }
}

void select::addOrder(const QList<QString>& cols,order sqlorder)
{
  if(!isOrder)
    {
      sql.append(" ORDER BY ");
      for(auto it=cols.begin();it!=cols.end();++it)
        {
          sql.append(*it);
          if((it+1)!=cols.end())sql.append(',');
        }
      switch(sqlorder)
        {
        case DESC:sql.append(" DESC");break;
        case ASC:sql.append(" ASC");break;
        }
      isOrder=true;
    }
}

// dbConn class
// use singleton mode

config * dbConn::conf = nullptr;
dbConn * dbConn::instance = nullptr;
unsigned int dbConn::threadCount = 4;

dbConn::dbConn(const QString &dbName)
{
  QString conSettings = "MYSQL_OPT_CONNECT_TIMEOUT=" + QString::number(conf->dbConnTimeOut())
      +";MYSQL_OPT_READ_TIMEOUT=" + QString::number(conf->dbReadTimeOut()) + ";MYSQL_OPT_WRITE_TIMEOUT="
      + QString::number(conf->dbWriteTimeOut());
  control = new dbWrapper::Control({"QMYSQL","DBConn",conf->dbHost(),dbName,conf->dbUname(),conf->dbPwd(),conSettings});
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
