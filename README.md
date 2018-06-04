# libserver
Library Mgmt Server Side Application

## 2018 Spring
<b>O</b>bject-<b>O</b>riented <b>P</b>rogramming Project

## TO-DO List
Almost done! Prepare for debug phase and beta release.

## Usage
Available commands:
```
Usage: ./libserver [option] <parameter>

Options:
  -i, --install              	 Install the libserver service
  -u, --uninstall            	 Uninstall the libserver service
  -s, --start <config-path>  	 Start the libserver service using the config file
  -t, --stop                 	 Stop the libserver service
  --status                   	 Show the status of libserver service
  --fake <config-path>       	 Run the libserver service in fake mode(for debugging purpose)
  -h, --help                 	 Display this help message
  --dbus-prefix <path>       	 Set the path for the installed dbus configuration file
  --initd-prefix <path>      	 Set the path for the installed init.d script
```

First you need to install the service, simply type install command, and this will create a configuration file in dbus
conf path(usually /etc/dbus-1/system.d/) and an init.d bash script in /etc/init.d/

Then create a file named libserver in /etc/init.d
```bash
#!/bin/bash
### BEGIN INIT INFO
# Provides:          libserver
# Required-Start:    $local_fs $syslog dbus
# Required-Stop:     $local_fs $syslog dbus
# Default-Start:     2 3 4 5
# Default-Stop:
# Short-Description: libserver
# Description:       Library Management Server
### END INIT INFO

HELP_TEXT="Usage: $0 {start|stop|restart|uninstall|force-reload|status|help}"

case "$1" in
    start)
       [installed path]/libserver --start [configFile path]
       ;;
    stop)
       [installed path]/libserver --stop
       ;;
    force-reload) # Just fall through
       ;&
    restart)
       [installed path]/libserver --stop
       [installed path]/libserver --start [configFile path]
       ;;
    uninstall)
       [installed path]/libserver --uninstall --initd-prefix=/etc/init.d --dbus-prefix=/etc/dbus-1/system.d
       ;;
    status)
       [installed path]/libserver --status
       ;;
    help)
       echo $HELP_TEXT
       ;;
    *)
       echo "Unrecognised option. "$HELP_TEXT
esac
```


## Third-party libraries or projects involved
[QtDaemon](https://bitbucket.org/nye/qtdaemon/overview)
[Epoll eventdispatcher](https://github.com/sjinks/qt_eventdispatcher_epoll)
[Smtp Email](https://github.com/bluetiger9/SmtpClient-for-Qt)
