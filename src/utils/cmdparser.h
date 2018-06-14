#ifndef CMDPARSER_H
#define CMDPARSER_H
#include <QString>

class cmdParser
{
public:
    enum state
    {
        success,
        invalid_cmd,
        failure
    };
private:
  state result = success;
  // original parameter
  int _argc;
  char **_argv;
  // converted parameter
  int argc;
  char **argv;
  // other stuffs
  QString serviceName;
  QString confPath;
  bool isStart;
  void usage();
  void parse();
public:
  state getParseResult()const;
  cmdParser(int _argc,char **_argv,const QString &serviceName);
  ~cmdParser();
  const QString& getConfPath();
  int& getArgc();
  char** getArgv();
  bool isStartCommand()const;
};

#endif // CMDPARSER_H
