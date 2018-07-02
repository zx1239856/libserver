# libserver
[![Build Status](https://travis-ci.org/zx1239856/libserver.svg?branch=master)](https://travis-ci.org/zx1239856/libserver) [![Build status](https://ci.appveyor.com/api/projects/status/ig4gmoo8mf317b9x?svg=true)](https://ci.appveyor.com/project/zx1239856/libserver) [![License: MIT](https://img.shields.io/badge/license-MIT-blue.svg)](https://github.com/zx1239856/libserver/blob/master/LICENSE)  

A cross-platform, versatile library management system 

Server side application

Tested on Ubuntu 14.04-18.04 , CentOS 7

It is recommended that you run the server on Linux to avoid weird problems.

If you have encountered any problem while using this application, see [Problem Resolutions](#problem-resolutions) below.

## 2018 Spring
**O**bject-**O**riented **P**rogramming Project
:+1::+1::+1:
See [contributors](https://github.com/zx1239856/libserver/graphs/contributors) here   
:sparkles::sparkles::sparkles:**The contributors of the whole project(Server and Client) are [here](https://github.com/zx1239856/libserver/wiki/Statistics-and-Collaboration)**

## Highlights

The icing on the cake would be as follows:

+ Book search/borrow/return
+ Online PDF preview
+ Client configuration(password, server address, etc.) encrypted storage
+ Administrator management
+ Automatic Email return notification
+ Find your forgotten password through Email
+ Easy customizable Email template
+ etc...

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

## Usage

The server requires MySQL as server-side database, so you need to set up a database server in advance either using the same host or some other remote host. But it is recommended that you deploy DB server locally to maximize performance and stability.

**Prepare your SQL server**

First you need to deploy MySQL server. Create a database named **libserver**, then import [libserver.sql](https://github.com/zx1239856/libserver/blob/master/sql/libserver.sql) file into this database. You can conveniently use [phpMyAdmin](https://www.phpmyadmin.net/) or other MySQL management software.

**Config File Guidance**

Before running, please create a config file (See example config file in libserver/docs/[config.conf](https://github.com/zx1239856/libserver/blob/master/docs/config.conf))

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

For **Ubuntu 17.10** or earlier, or any other linux system using initd to manage services other than systemd: 

Now you would be able to manage the server using systemctl.

### A little reminder

For **CentOS** or **Ubuntu 18.04**, you need extra procedures:

*Step1*: Create a service file in (**CentOS**) /usr/lib/systemd/system, (**Ubuntu**) /etc/systemd/system, you may type

```bash
vim /usr/lib/systemd/system/libserver.service  ## CentOS
vim /etc/systemd/system/libserver.service  ## Ubuntu
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



### Email Sending Customization

The server supports notifying users to return books and password reset through Email. You can choose to use either plain text Email or one equipped with HTML, which would be much more beautiful.

To customize your Email, please create a folder named **template** in the same directory as the server App. And create two HTML files, **forgetPwd.html** for password reset, **returnNotify.html** for book return notification.  The forgetPwd.html should contain **%%Newpwd** placeholder, and returnNotify.html should contain **%%Bookname** and **%%Exptime**.

You can refer to [example HTML](https://github.com/zx1239856/libserver/tree/master/template) here.



## Problem Resolutions

Here is guidance to some common problems.

**Q**: QSqlDatabase: QMYSQL driver not loaded

**A**: This problem comes from the version of libmysqlclient.so. Typically, MySQL drivers of QT support version 18 of this library. If you have versions that are not identical, the MySQL would not load.

To solve this, you need to install [RPM](https://pkgs.org/download/libmysqlclient.so.18) or [DEB](https://launchpad.net/ubuntu/xenial/amd64/libmysqlclient18/5.6.25-0ubuntu1) package, and the problem should be addressed.

**Q**: QSSLSocket unavailable on Ubuntu 18.04, resulting in the failure of email sending

**A**: Owing to the fact that the newer Ubuntu release utilizes a newer version of OpenSSL which is not compatible with SSLSocket protocol, the email sending would presumably fail. To solve this, please install the following packages:

```bash
sudo apt install libssl1.0-dev
```



## Third-party libraries or projects involved

The server is based on Qt5, and it involves these libraries/ projects as well:

[QtDaemon](https://bitbucket.org/nye/qtdaemon/overview)  Modified to support user specified log path

[Epoll eventdispatcher](https://github.com/sjinks/qt_eventdispatcher_epoll)  Epoll event dispatcher for Qt4 and Qt5 

[Smtp Email](https://github.com/bluetiger9/SmtpClient-for-Qt)  Little changes to make the interface easier to utilize

[Poppler library](https://poppler.freedesktop.org/)  A library to handle PDF files, including Qt5 wrapper



## Wiki

For more information, please refer to [Wiki](https://github.com/zx1239856/libserver/wiki).
