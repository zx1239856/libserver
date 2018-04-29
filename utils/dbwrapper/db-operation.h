#pragma once

#include <QtConcurrent/QtConcurrent>
#include <QSqlError>
#include "db-wrapper.h"
#include "../config.h"

// SQL-query constructor basic class
class basicSQL
{
protected:
    QString sql;
public:
    basicSQL();
    basicSQL(const QString &_sql);
    virtual ~basicSQL()=0;
};

// derived specific SQL queries
class CreateDataBase:public basicSQL
{

};

class DropDataBase:public basicSQL
{

};

class CreateTable:public basicSQL
{

};

class DropTable:public basicSQL
{

};



class dbConn
{
private:
  dbWrapper::Control control; //({"QMYSQL", "DBconn", conf.dbHost, "test", "root", conf.dbPwd})
  unsigned int threadCount;
public:
  dbConn(const config &conf);  // initialize dbConn with configuration file
  dbConn(const dbConn &)=delete;
};
