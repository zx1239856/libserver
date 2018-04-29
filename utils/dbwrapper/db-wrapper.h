#pragma once

#include <functional>
#include <QtCore>
#include <QtSql>

/*
 * A simple wrapper for QSqlDatabase class
 * with multi-threading support
 */

// PropertyDeclare Start
// Useful Macro to declare private Var and public corresponding set function to modify it
#define PropertyDeclare(Type, Name, setName, ...)                   \
    private:                                                        \
    Type m_ ## Name __VA_ARGS__;                                    \
    public:                                                         \
    inline const Type &Name(void) const { return m_ ## Name; }      \
    inline void setName(const Type &Name) { m_ ## Name = Name; }    \
    private:
// PropertyDeclare End

namespace dbWrapper
{

enum QueryMode { QueryAutoMode, QueryMultiMode, QuerySingleMode };

// Database Settings
class DatabaseSettings
{
private:
    PropertyDeclare(QString, databaseType, setDatabaseType)
    PropertyDeclare(QString, connectionName, setConnectionName)

    PropertyDeclare(QString, DBHost, setDBHost)
    PropertyDeclare(QString, DBName, setDBName)
    PropertyDeclare(QString, DBUserName, setDBUserName)
    PropertyDeclare(QString, DBPassword, setDBPassword)
public:
    DatabaseSettings(const QString &databaseType, const QString &connectionName,
                     const QString &DBHost, const QString &DBName,
                     const QString &DBUserName, const QString &DBPassword);
};

// Connection Settings
class ConnectSettings
{
private:
    PropertyDeclare(int, maxOpenTime, setMaxOpenTime)
    PropertyDeclare(QueryMode, queryMode, setQueryMode)
    PropertyDeclare(int, minWaitTime, setMinWaitTime)

public:
    ConnectSettings(const int &maxOpenTime = 60 * 1000, const QueryMode &queryMode = QueryAutoMode, const int &minWaitTime = -1);
};

// Actual DB Query
class Query
{
private:
    QSqlQuery *m_query;
    QMutex *m_mutex = NULL;

public:
    Query(QSqlDatabase &dataBase, QMutex *mutex = NULL);
    Query(Query &&other);
    ~Query();
    inline QSqlQuery *operator->() { return m_query; }
    inline QSqlQuery &operator*() { return *m_query; }
};

// Connection Pool
class ConnectNode: public QObject
{
     Q_OBJECT

private:
    QSqlDatabase *m_database = NULL;
    QString m_connectionName;

    DatabaseSettings m_dataBaseSettings;
    ConnectSettings m_connectSettings;

    QTimer *m_autoClose = NULL;
    QMutex *m_mutex = NULL;

public:
    ConnectNode(const DatabaseSettings &dataBaseSettings, const ConnectSettings &connectSettings);
    ~ConnectNode();
public:
    Query query();
public slots:
    bool addDataBase();
    void removeDataBase();
    bool open();
    void close();
signals:
    void controlStartAutoClose();
    void controlStopAutoClose();
};

// Main Connection Controller
class Control
{
private:
    DatabaseSettings m_databaseSettings;
    ConnectSettings m_connectSettings;

    QMap<qint64, ConnectNode *> m_node;

    QMutex m_mutex;
    QTime *m_wait = NULL;

public:
    Control(const DatabaseSettings &databaseSettings, const ConnectSettings &connectSettings = ConnectSettings());
    Control(const Control &) = delete;
    ~Control();

public:
    void removeAll();
    Query query();

private:
    void insert(const qint64 &key);
};

}
