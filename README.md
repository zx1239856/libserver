# libserver
[![Build Status](https://travis-ci.org/zx1239856/libserver.svg?branch=master)](https://travis-ci.org/zx1239856/libserver) [![Build status](https://ci.appveyor.com/api/projects/status/ig4gmoo8mf317b9x?svg=true)](https://ci.appveyor.com/project/zx1239856/libserver) [![License: MIT](https://img.shields.io/badge/license-MIT-blue.svg)](https://github.com/zx1239856/libserver/blob/master/LICENSE)  

A cross-platform, versatile library management system 

Server side application

Tested on Ubuntu 14.04-18.04 , CentOS 7 , Windows 10(MSVC)

**Warning**: Ubuntu 18.04 uses a newer version of OpenSSL(OpenSSL 1.1.0g  2 Nov 2017), which is not compatible with SSLSocket of Qt, and thus you won't be able to send Email via SSL if you are using Ubuntu 18.04

For Windows platform, owing to the infirmities of Poppler-Qt5 library, we currently do not support PDF file handling. If you want to use this function, feel free to embrace Linux system such as Ubuntu.

## 2018 Spring
**O**bject-**O**riented **P**rogramming Project

See [contributors](https://github.com/zx1239856/libserver/graphs/contributors) here

## How to build

Make sure you have properly configured your Qt5 environment, or the project won't build.

**Requires Qt 5.8 or higher**

Also you need to install [Poppler library](https://poppler.freedesktop.org/). For Ubuntu, run the following command:

```bash
sudo apt-get install libpoppler-dev
```

For CentOS or other linux systems, you need to build the library yourself, and we provide you with a simple script to compile and install the library(Assume you have cloned our project):

```bash
cd libserver/scripts
chmod +x install_poppler_CentOS.sh
./install_poppler_CentOS.sh
```

If everything goes without error and you have successfully acquired all the libraries needed, run the following command to build the project:

```bash
git clone https://github.com/zx1239856/libserver.git
cd libserver && qmake && make
```

#### Build on Windows

Assume you have installed Qt creator, so gently open libserver.pro file with that. Now, don't tell me about your unawareness of the building procedure!

## Usage

The server requires MySQL as server-side database, so you need to set up a database server in advance either using the same host or some other remote host. But it is recommended that you deploy DB server locally to maximize performance and stability.

Before running, please create a config file (See example config file in libserver/docs/config.conf)

Available commands for the server:

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

First you need to install the service, simply type install command, and this will create a configuration file in dbus conf path(usually /etc/dbus-1/system.d/) and an init.d bash script in /etc/init.d/

The init.d script is as follows, please replace the [configFile path] with your real configuration file path.

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

Now you would be able to manage the server using systemctl.

### A little reminder

For **CentOS**, you need extra procedures:

*Step1*: Create a service file in /usr/lib/systemd/system, you may type

```bash
vim /usr/lib/systemd/system/libserver.service
```

The content of the service file is

```bash
[Unit]
Description=libserver - library management server side application
After=network.target remote-fs.target nss-lookup.target

[Service]
Type=forking
ExecStart=/etc/init.d/libserver start
ExecReload=/etc/init.d/libserver restart
ExecStop=/etc/init.d/libserver stop
User=root
Group=root
Environment="LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib64:/usr/local/lib"

[Install]
WantedBy=multi-user.target
```

*Step2*: 

```bash
systemctl daemon-reload
```

Now you will be able to manage the service using systemctl just as in the Ubuntu.

### Systemctl guidance

Here are commands used to manage the service through systemctl, which is a commonly used application to monitor and manage services in most Linux systems.

```bash
systemctl start libserver # Start the server
systemctl stop libserver  # Stop the server
systemctl restart libserver  # Restart the server
systemctl status libserver # Get the status of the server
systemctl enable libserver # Automatic startup the server when system boots
```



## Third-party libraries or projects involved

The server is based on Qt5, and it involves these libraries/ projects as well:

[QtDaemon](https://bitbucket.org/nye/qtdaemon/overview)  Modified to support user specified log path

[Epoll eventdispatcher](https://github.com/sjinks/qt_eventdispatcher_epoll)  Epoll event dispatcher for Qt4 and Qt5 

[Smtp Email](https://github.com/bluetiger9/SmtpClient-for-Qt)  Little changes to make the interface easier to utilize

[Poppler library](https://poppler.freedesktop.org/)  A library to handle PDF files, including Qt5 wrapper
