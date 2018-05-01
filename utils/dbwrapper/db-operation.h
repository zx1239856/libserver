#pragma once

#include <QtConcurrent/QtConcurrent>
#include <QSqlError>
#include <vector>
#include "db-wrapper.h"
#include "../config.h"

namespace sql
{
  // SQL-query constructor basic class
  class basicSQL
  {
  protected:
    QString sql;
    static dbWrapper::Control *mainDBControl;
  public:
    basicSQL(){}
    //basicSQL(const QString &_sql);
    virtual void exec(){}
    virtual ~basicSQL(){}
  };

  /* derived specific SQL queries
   *
   */
  // USAGE: insert(tablename,fields,vals); insert->exec();
  // This can either take a single row or a batch of rows
  class insert:public basicSQL
  {
  private:
    const QString tablename;
    const std::vector<QString> &fields;
    const std::vector<QVariantList> &vals;
  public:
    insert(const QString &_tableName,
           const std::vector<QString> &_fields,
           const std::vector<QVariantList> &_vals);
    virtual void exec();
  };

  class update:public basicSQL
  {
  private:
    const QString tablename;
    const QString filter;
  public:
    update(const QString& _tablename,
           const QString& _filter);
    virtual void exec();
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
