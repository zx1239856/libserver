#ifndef SQLARRAY_H
#define SQLARRAY_H

#include <QString>
#include <QVector>
// a simple handler converting stringList or array to string so as to store
// them in SQL database

class sqlArray
{
public:
  static QString Array2String(const QStringList& arr,const QChar& split='@');
  static QStringList String2Array(const QString& str,const QChar& split='@');
  static QString Array2String(QVector<int>& arr,bool sort = false, const QChar& split='@');
  static QVector<int> String2Array(const QString& str,bool sort = false, const QChar& split='@');
};

#endif // SQLARRAY_H
