#pragma once

#include <QtConcurrent/QtConcurrent>
#include <QSqlError>
#include <QVector>
#include <QSqlRecord>
#include <QMap>
#include "db-wrapper.h"
#include "../config.h"

namespace sql
{
  // SQL-query constructor basic class
  // This can run direct SQL sentence
  class basicSQL:public QObject
  {
    Q_OBJECT
  protected:
    dbWrapper::Query query;
    // here we use vector in STL since push_back of QVector
    // does not support rvalue reference
    QString sql;
    QSqlError err;
    static dbWrapper::Control *mainDBControl;
  public:
    basicSQL();
    basicSQL(const QString &s);
    static void setControl(dbWrapper::Control *c);
    virtual bool exec();
    QSqlError lastError();
    virtual QVector<QSqlRecord> toResult();
    virtual ~basicSQL(){}
  signals:
    void onSuccess();
    void onFail(const QSqlError &err);
    void onResult(const QVector<QSqlRecord> &res);
  };

  /* derived specific SQL queries
   *
   */
  // USAGE: insert(tablename,fields,vals); insert->exec();
  // INSERT INTO tablename (fields) VALUES (vals)
  class insert:public basicSQL
  {
  private:
    const QMap<QString,QVariant>& cont;
  public:
    insert(const QString &tableName, const QMap<QString,QVariant> &content);
    virtual bool exec();
  };

  // update(tablename,content,condition)
  // UPDATE tablename a=a1,b=b1,... WHERE condition
  class update:public basicSQL
  {
  private:
    const QMap<QString,QVariant> &cont;
  public:
    update(const QString& tablename,
           QMap<QString,QVariant>& content,
           const QString& condition);
    virtual bool exec();
  };

  // delete(tablename,conidtion)
  // DELETE FROM tablename WHERE condition
  class del:public basicSQL
  {
  public:
    del(const QString &tablename,
        const QString &condition);
  };

  // select(what,tablename,condition)
  // SELECT what FROM tablename WHERE condition /
  // SELECT DISTINCT what FROM ... /
  // SELECT ... LIMIT [number of records] /
  // SELECT ... LIMIT [start pos, number of records] /
  // SELECT ... ORDERED BY a,b,c DESC/ASC /
  // SELECT CONCAT(col,',',col) FROM ...
  // SELECT AVG/SUM(col) FROM ... /
  // SELECT MIN(col) FROM ... /
  enum order{DESC,ASC};
  enum func{MIN,MAX,AVG,SUM,COUNT};

  class select:public basicSQL
  {
    bool isOrder=false;
    bool isLimit=false;
  public:
    select(const QString& tablename,const QString &condition,const QList<QString> &what={"*"},bool isDistinct=false);
    select(const QString& tablename,const QList<QString> &what={"*"},bool isDistinct=false);
    select(const QString& tablename,const QString &col, func sqlfunc);
    void addLimit(uint limit,uint start_pos);
    void addOrder(const QString& colname, order sqlorder);
    void addOrder(const QList<QString>& cols,order sqlorder);
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
  static unsigned int threadCount;
  // auto destroy singleton itself
  class dbConnGarbo
  {
  public:
    ~dbConnGarbo()
    {
      if(dbConn::instance)delete dbConn::instance;
    }
  };
  static dbConnGarbo Garbo;
  dbConn(const QString &dBName = "");  // initialize dbConn with configuration file
  ~dbConn();
public:
  dbConn(const dbConn &)=delete;
  dbConn& operator=(dbConn &)=delete;
  static void setConf(config* conf);
  static void setMaxThread(unsigned int t);
  static dbConn* getInstance();
  dbWrapper::Control* getControl();
};

