#pragma once

#include <QtConcurrent/QtConcurrent>
#include <QSqlError>
#include <vector>
#include <QMap>
#include "db-wrapper.h"
#include "../config.h"

namespace sql
{
  // SQL-query constructor basic class
  // This can run direct SQL sentence
  class basicSQL
  {
  protected:
    dbWrapper::Query query;
    QString sql;
    static dbWrapper::Control *mainDBControl;
  public:
    basicSQL();
    basicSQL(const QString &s);
    //basicSQL(const QString &_sql);
    virtual bool exec();
    virtual QSqlError lastError();
    virtual ~basicSQL(){}
  };

  /* derived specific SQL queries
   *
   */
  // USAGE: insert(tablename,fields,vals); insert->exec();
  // This can either take a single row or a batch of rows
  // INSERT INTO tablename (fields) VALUES (vals)
  class insert:public basicSQL
  {
  private:
    const QString tablename;
    const QMap<QString,QVariantList> &content;
  public:
    insert(const QString &_tableName,
           const QMap<QString,QVariantList> &_content);
    virtual bool exec();
  };

  // update(tablename,content,filter)
  // UPDATE tablename a=a1,b=b1,... WHERE filter
  class update:public basicSQL
  {
  private:
    const QString tablename;
    const QMap<QString,QVariant> &content;
    const QString filter;
  public:
    update(const QString& _tablename,
           QMap<QString,QVariant>& _content,
           const QString& _filter);
    virtual bool exec();
  };

  // delete(tablename,filter)
  // DELETE FROM tablename WHERE filter
  class del:public basicSQL
  {
  private:
    const QString tablename;
    const QString filter;
  public:
    del(const QString &_tablename,
        const QString &_filter);
    virtual bool exec();
  };

  // select(what,tablename,filter)
  // SELECT what FROM tablename WHERE filter
  class select:public basicSQL
  {
  private:
    const QVector<QString> &what={"*"};
    const QString tablename;
    const QString filter;
  public:
    select(const QString& _tablename,const QString _filter="1");
    select(const QVector<QString> &_what,const QString& _tablename,const QString _filter="1");
    virtual bool exec();
    bool next();
    QVariant value(int i);
  };
}


// Main Database Connection
// Use singleton mode
class dbConn
{
private:
  static dbConn* instance;
  static config* conf;
  dbWrapper::Control *control; //({"QMYSQL", "DBconn", conf.dbHost, "test", "root", conf.dbPwd})
  unsigned int threadCount;

  // auto destroy singleton itself
  class dbConnGarbo
  {
  public:
    ~dbConnGarbo()
    {
      if(dbConn::instance)delete dbConn::instance;
    }
  };
  dbConn(unsigned int threads = 4 , const QString &dBName = "");  // initialize dbConn with configuration file
  dbConn(const dbConn &)=delete;
  dbConn& operator=(dbConn &)=delete;
  ~dbConn();
public:
  static dbConn* getInstance();
  dbWrapper::Control* getControl();
};
