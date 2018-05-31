#include "sqlarray.h"
#include <QStringList>

QString sqlArray::Array2String(const QStringList &arr, const QChar &split)
{
  QString result;
  for(auto it=arr.begin();it!=arr.end();++it)
    {
      result.append(*it);
      if((it+1)!=arr.end())result.append(split);
    }
  return result;
}

QStringList sqlArray::String2Array(const QString &str, const QChar &split)
{
  if(str.isEmpty())
    {
      return  QStringList();
    }
  else return str.split(split);
}

QString sqlArray::Array2String(QVector<int> &arr, bool sort, const QChar &split)
{
  QString result;
  if(sort)
    {
      std::sort(arr.begin(),arr.end());
    }
  for(auto it=arr.begin();it!=arr.end();++it)
    {
      result.append(QString::number(*it));
      if((it+1)!=arr.end())result.append(split);
    }
  return result;
}

QVector<int> sqlArray::String2Array(const QString& str,bool sort, const QChar& split)
{
  QVector<int> result;
  QStringList &&lst = String2Array(str,split);
  for(auto it=lst.begin();it!=lst.end();++it)
    {
      result.push_back(it->toInt());
    }
  if(sort)
    {
      std::sort(result.begin(),result.end());
    }
  return result;
}
