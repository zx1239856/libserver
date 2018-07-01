#ifndef DBLOG_H
#define DBLOG_H
class QString;

class dbLog
{
public:
  enum operatorType
  {
    staff,
    reader,
    general
  };
  dbLog();
  static void log(const QString type, const QString content, unsigned int operatorID, operatorType opType);
};

#endif // DBLOG_H
