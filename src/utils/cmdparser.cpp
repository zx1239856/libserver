#include "cmdparser.h"
#include <stdio.h>
#include <QStringList>

cmdParser::cmdParser(int argCount,char **argContent,const QString &serv):
  _argc(argCount),_argv(argContent),argc(0),argv(nullptr),serviceName(serv)
{
  parse();
}

cmdParser::~cmdParser(){}

void cmdParser::usage()
{
  QByteArray ba = serviceName.toLatin1();
  const char* srv = ba.data();
  const char* commands[] = {"-i, --install","-u, --uninstall",
                            "-s, --start <config-path>","-t, --stop","--status","--fake <config-path>",
                            "-h, --help","--dbus-prefix <path>","--initd-prefix <path>"};
  printf("Usage: %s [option] <parameter>\n\n",_argv[0]);
  printf("Options:\n  %-26s \t Install the %s service\n"
         "  %-26s \t Uninstall the %s service\n"
         "  %-26s \t Start the %s service using the config file\n"
         "  %-26s \t Stop the %s service\n"
         "  %-26s \t Show the status of %s service\n",
         commands[0],srv,commands[1],srv,commands[2],srv,commands[3],srv,commands[4],srv);
#ifdef DEBUG_MODE
  printf("  %-26s \t Run the %s service in fake mode(for debugging purpose)\n",commands[5],srv);
#endif
  printf("  %-26s \t Display this help message\n"
   "  %-26s \t Set the path for the installed dbus configuration file\n"
   "  %-26s \t Set the path for the installed init.d script\n",commands[6],commands[7],commands[8]);
}

void cmdParser::parse()
{
  if(_argc<=0)
    {
      printf("\nInternal error: argc smaller than one occured.\n");
      exit(EXIT_FAILURE);
    }
  if(_argc==1)
    {
      usage();
      exit(EXIT_SUCCESS);
    }
  else if(_argc==2)
    {
      QString str(_argv[1]);
      bool okay=false;
      if(str=="-i"||str=="--install")okay=true;
      else if(str=="-u"||str=="--uninstall")okay=true;
      else if(str=="-t"||str=="--stop")okay=true;
      else if(str=="--status")okay=true;
      else if(str=="-h"||str=="--help")okay=true;
      else if(str=="-s"||str=="--start")okay=false;
#ifdef DEBUG_MODE
      else if(str=="--fake")okay=false;
#endif
      else if(str=="--dbus-prefix"||str=="--initd-prefix")okay=false;
      else
        {
          printf("%s %s",_argv[0],_argv[1]);
          printf("\nUnrecognized command. Please retry.\n");
          usage();
          exit(EXIT_SUCCESS);
        }
      if(okay)
        {
          // direct conversion is okay
          argc=_argc;
          argv=_argv;
          return;
        }
      else
        {
          printf("\nPlease specify a parameter for the command. \n");
          usage();
          exit(EXIT_SUCCESS);
        }
    }
  else if(_argc==3)
    {
      QString str(_argv[1]);
      if(str=="-s"||str=="--start"||str=="-d")
        {
          argc=_argc;
          argv=_argv;
          confPath=QString(_argv[2]);
          return;
        }
#ifdef DEBUG_MODE
      else if(str=="--fake")
        {
          argc=_argc-1; // here you need to parse only --fake to internal
          argv=_argv;
          confPath=QString(_argv[2]);
          return;
        }
#endif
      else if(str=="--dbus-prefix"||str=="--initd-prefix")
        {
          argc=_argc;
          argv=_argv;
          return;
        }
      else
        {
          printf("\nUnrecognized command. Please retry.\n");
          usage();
          exit(EXIT_SUCCESS);
        }
    }
  else
    {
      printf("\nExtra invalid command detected. Please retry.\n");
      usage();
      exit(EXIT_SUCCESS);
    }
}

const QString & cmdParser::getConfPath()
{
  return confPath;
}

char** cmdParser::getArgv()
{
  return argv;
}

int& cmdParser::getArgc()
{
  return argc;
}
