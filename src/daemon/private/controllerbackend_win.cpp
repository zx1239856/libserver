/****************************************************************************
**
** Copyright (C) 2016 Konstantin Shegunov <kshegunov@gmail.com>
**
** This file is part of the QtDaemon library.
**
** The MIT License (MIT)
**
** Permission is hereby granted, free of charge, to any person obtaining a copy
** of this software and associated documentation files (the "Software"), to deal
** in the Software without restriction, including without limitation the rights
** to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
** copies of the Software, and to permit persons to whom the Software is
** furnished to do so, subject to the following conditions:
**
** The above copyright notice and this permission notice shall be included in all
** copies or substantial portions of the Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
** OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
** SOFTWARE.
**
****************************************************************************/

#include "controllerbackend_win.h"
#include "qdaemonapplication.h"
#include "qdaemonlog.h"

#include <QtCore/qcommandlineparser.h>
#include <QtCore/qelapsedtimer.h>
#include <QtCore/qdir.h>
#include <QtCore/qthread.h>

#include <Windows.h>

QT_BEGIN_NAMESPACE

using namespace QtDaemon;

// -------------------------------------------------------------------------------------------------------------------------------------------------------- //
// --- Utility classes for windows ------------------------------------------------------------------------------------------------------------------------ //
// -------------------------------------------------------------------------------------------------------------------------------------------------------- //

static const qint32 serviceNotifyTimeout = 30000;			// Up to 30 seconds

class WindowsService;
class WindowsServiceManager
{
    friend class WindowsService;

private:
    WindowsServiceManager(SC_HANDLE manager)
        : handle(manager)
    {
    }

public:
    ~WindowsServiceManager()
    {
        close();
    }

    inline bool isValid()
    {
        return handle;
    }

    static WindowsServiceManager open()
    {
        // Open a handle to the service manager
        SC_HANDLE manager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
        if (!manager)
            qDaemonLog(QStringLiteral("Couldn't open a handle to the service manager (Error code %1)").arg(GetLastError()), QDaemonLog::ErrorEntry);

        return WindowsServiceManager(manager);
    }

    void close()
    {
        if (handle && !CloseServiceHandle(handle))
            qDaemonLog(QStringLiteral("Error while closing the service manager (Error code %1).").arg(GetLastError()), QDaemonLog::WarningEntry);

        handle = Q_NULLPTR;
    }

private:
    SC_HANDLE handle;
};

class WindowsService
{
public:
    WindowsService(const QString & name, WindowsServiceManager & sm)
        : manager(sm), handle(Q_NULLPTR), serviceName(name)
    {
    }

    WindowsService(WindowsServiceManager & sm)
        : manager(sm), handle(Q_NULLPTR)
    {
    }

    ~WindowsService()
    {
        close();
    }

    inline void setName(const QString & name)
    {
        serviceName = name;
    }

    inline QString name() const
    {
        return serviceName;
    }

    inline void setDescription(const QString & description)
    {
        serviceDescription = description;
    }

    inline QString description() const
    {
        return serviceDescription;
    }

    inline void setExecutable(const QString & executable)
    {
        serviceExecutable = executable;
    }

    inline QString executable() const
    {
        return serviceExecutable;
    }

    inline bool isOpen()
    {
        return handle;
    }

    bool open(int access)
    {
        LPCTSTR lpName = reinterpret_cast<LPCTSTR>(serviceName.utf16());

        // Open the service
        handle = OpenService(manager.handle, lpName, access);
        if (!handle)  {
            DWORD error = GetLastError();
            switch (error)
            {
            case ERROR_ACCESS_DENIED:
                qDaemonLog(QStringLiteral("Couldn't open service control, access denied."), QDaemonLog::ErrorEntry);
                break;
            case ERROR_SERVICE_DOES_NOT_EXIST:
                qDaemonLog(QStringLiteral("The service is not installed."), QDaemonLog::ErrorEntry);
                break;
            default:
                qDaemonLog(QStringLiteral("Couldn't open the service (Error code %1).").arg(error), QDaemonLog::ErrorEntry);
            }
        }

        return handle;
    }

    void close()
    {
        if (handle && !CloseServiceHandle(handle))
            qDaemonLog(QStringLiteral("Error while closing the service handle (Error code %1).").arg(GetLastError()), QDaemonLog::WarningEntry);

        handle = Q_NULLPTR;
    }

    bool create()
    {
        LPCTSTR lpName = reinterpret_cast<LPCTSTR>(serviceName.utf16());
        LPCTSTR lpPath = reinterpret_cast<LPCTSTR>(serviceExecutable.utf16());

        // Create the service
        handle = CreateService(manager.handle, lpName, lpName, SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS, SERVICE_AUTO_START, SERVICE_ERROR_NORMAL, lpPath, NULL, NULL, NULL, NULL, NULL);
        if (!handle)  {
            DWORD error =  GetLastError();
            switch (error)
            {
            case ERROR_ACCESS_DENIED:
                qDaemonLog(QStringLiteral("Couldn't install the service, access denied."), QDaemonLog::ErrorEntry);
                break;
            case ERROR_DUPLICATE_SERVICE_NAME:
            case ERROR_SERVICE_EXISTS:
                qDaemonLog(QStringLiteral("The service was already installed (Uninstall first)."), QDaemonLog::ErrorEntry);
                break;
            default:
                qDaemonLog(QStringLiteral("Couldn't install the service (Error code %1).").arg(error), QDaemonLog::ErrorEntry);
            }

            return false;
        }

        // Set the description if provided
        if (!serviceDescription.isEmpty())  {
            LPTSTR lpDescription = reinterpret_cast<LPTSTR>(const_cast<ushort *>(serviceDescription.utf16()));		// MS doesn't care about constness

            SERVICE_DESCRIPTION serviceDescription;
            serviceDescription.lpDescription = lpDescription;
            if (!ChangeServiceConfig2(handle, SERVICE_CONFIG_DESCRIPTION, &serviceDescription))
                qDaemonLog(QStringLiteral("Couldn't set the service description (Error code %1).").arg(GetLastError()), QDaemonLog::WarningEntry);
        }

        return true;
    }

    bool remove()
    {
        if (!handle)  {
            qWarning("Service must be opened before calling remove().");
            return false;
        }

        // Delete the service
        if (!DeleteService(handle))  {
            DWORD error = GetLastError();
            switch (error)
            {
            case ERROR_SERVICE_MARKED_FOR_DELETE:
                qDaemonLog(QStringLiteral("The service had already been marked for deletion."), QDaemonLog::ErrorEntry);
                break;
            default:
                qDaemonLog(QStringLiteral("Couldn't delete the service (Error code %1).").arg(error), QDaemonLog::ErrorEntry);
            }

            return false;
        }

        return true;
    }

    bool start()
    {
        if (!handle)  {
            qWarning("Service must be opened before calling start().");
            return false;
        }

        // Start the service
        if (!StartService(handle, 0, NULL))  {
            DWORD error = GetLastError();
            switch (error)
            {
            case ERROR_ACCESS_DENIED:
                qDaemonLog(QStringLiteral("Couldn't stop the service: Access denied."), QDaemonLog::ErrorEntry);
                break;
            case ERROR_PATH_NOT_FOUND:
                qDaemonLog(QStringLiteral("Couldn't start the service: The path to the executable is set incorrectly."), QDaemonLog::ErrorEntry);
                break;
            case ERROR_SERVICE_ALREADY_RUNNING:
                qDaemonLog(QStringLiteral("The service is already running."), QDaemonLog::ErrorEntry);
                break;
            case ERROR_SERVICE_REQUEST_TIMEOUT:
                qDaemonLog(QStringLiteral("The service is not responding."), QDaemonLog::ErrorEntry);
                break;
            default:
                qDaemonLog(QStringLiteral("Couldn't start the service (Error code %1).").arg(error), QDaemonLog::ErrorEntry);
            }

            return false;
        }

        return waitForStatus(SERVICE_RUNNING);

    }

    bool stop()
    {
        if (!handle)  {
            qWarning("Service must be opened before calling stop().");
            return false;
        }

        // Stop the service
        SERVICE_STATUS serviceStatus;
        if (!ControlService(handle, SERVICE_CONTROL_STOP, &serviceStatus))  {
            DWORD error = GetLastError();
            switch (error)
            {
            case ERROR_ACCESS_DENIED:
                qDaemonLog(QStringLiteral("Couldn't stop the service, access denied."), QDaemonLog::ErrorEntry);
                break;
            case ERROR_SERVICE_NOT_ACTIVE:
                qDaemonLog(QStringLiteral("The service is not running."), QDaemonLog::ErrorEntry);
                break;
            case ERROR_SERVICE_REQUEST_TIMEOUT:
                qDaemonLog(QStringLiteral("The service is not responding."), QDaemonLog::ErrorEntry);
                break;
            default:
                qDaemonLog(QStringLiteral("Couldn't stop the service (Error code %1).").arg(error), QDaemonLog::ErrorEntry);
            }

            return false;
        }

        return waitForStatus(SERVICE_STOPPED);
    }

    DWORD status()
    {
        if (!handle)  {
            qWarning("Service must be opened before calling status().");
            return 0;
        }

        DWORD bytesNeeded;
        SERVICE_STATUS_PROCESS serviceStatus;
        if (!QueryServiceStatusEx(handle, SC_STATUS_PROCESS_INFO, reinterpret_cast<LPBYTE>(&serviceStatus), sizeof(SERVICE_STATUS_PROCESS), &bytesNeeded))  {
            DWORD error = GetLastError();
            switch (error)
            {
            case ERROR_ACCESS_DENIED:
                qDaemonLog(QStringLiteral("Couldn't get the service's status, access denied."), QDaemonLog::ErrorEntry);
                break;
            default:
                qDaemonLog(QStringLiteral("Couldn't get the service's status (Error code %1).").arg(error), QDaemonLog::ErrorEntry);
            }

            return 0;
        }

        return serviceStatus.dwCurrentState;
    }

private:
    bool waitForStatus(DWORD requestedStatus)
    {
        const unsigned long pollInterval = serviceNotifyTimeout / 20;		// A twentieth of the allowed timeout interval (in ms)

        // Do polling (Windows XP compatible)
        QElapsedTimer timeout;
        timeout.start();
        while (!timeout.hasExpired(serviceNotifyTimeout))  {
            if (status() == requestedStatus)
                return true;

            // Wait a bit before trying out
            QThread::msleep(pollInterval);
        }

        qDaemonLog(QStringLiteral("The service is not responding."), QDaemonLog::ErrorEntry);
        return false;
    }

private:
    WindowsServiceManager & manager;
    SC_HANDLE handle;
    QString serviceName;
    QString serviceDescription;
    QString serviceExecutable;
};

class WindowsSystemPath
{
public:
    WindowsSystemPath()
    {
        // Retrieve the system path (RegOpenKeyTransacted breaks compatibility with Windows XP)
        if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, pathRegistryKey, 0, KEY_QUERY_VALUE | KEY_SET_VALUE, &registryKey) != ERROR_SUCCESS)  {
            qDaemonLog(QStringLiteral("Couldn't open the PATH registry key. You may need to update the system path manually."), QDaemonLog::WarningEntry);
            registryKey = Q_NULLPTR;
        }
    }

    ~WindowsSystemPath()
    {
        if (registryKey && RegCloseKey(registryKey) != ERROR_SUCCESS)
            qDaemonLog(QStringLiteral("Couldn't close the PATH registry key. You may need to update the system path manually."), QDaemonLog::WarningEntry);
    }

    bool addEntry(const QString & dir)
    {
        if (!load())
            return false;

        QString path = QDir::cleanPath(dir);
        if (entries.contains(path, Qt::CaseInsensitive))
            return true;

        entries.append(path);
        return save();
    }

    bool removeEntry(const QString & dir)
    {
        if (!load())
            return false;

        QString path = QDir::cleanPath(dir);
        for (QStringList::Iterator i = entries.begin(), end = entries.end(); i != end; i++)  {		// Qt doesn't have case insensitive QStringList::removeAll()
            if (i->compare(path, Qt::CaseInsensitive) == 0)
                entries.erase(i);
        }

        return save();
    }

private:
    bool load()
    {
        if (!registryKey)
            return false;

        // Get the size
        DWORD bufferSize;
        if (RegQueryValueEx(registryKey, TEXT("Path"), 0, NULL, NULL, &bufferSize) != ERROR_SUCCESS)  {
            qDaemonLog(QStringLiteral("Couldn't retrieve the PATH registry key' size."), QDaemonLog::WarningEntry);
            return false;
        }

        // Allocate enough to contain the PATH
        LPBYTE buffer = new BYTE[bufferSize];

        // Repeat the query, this time with a buffer to get the actual data
        if (RegQueryValueEx(registryKey, TEXT("Path"), 0, NULL, buffer, &bufferSize) != ERROR_SUCCESS)  {
            qDaemonLog(QStringLiteral("Couldn't retrieve the PATH registry key. You may need to update the system path manually."), QDaemonLog::WarningEntry);
            delete [] buffer;
            return false;
        }

        // Continuing to chop any type safety away
        QString path(reinterpret_cast<const QChar * const>(buffer), (bufferSize - 1) / sizeof(TCHAR) );

        // Normalize the paths
        entries = path.split(';', QString::SkipEmptyParts);
        for (QStringList::Iterator i = entries.begin(), end = entries.end(); i != end; i++)
            *i = QDir::cleanPath(*i);

        // Free up the buffer
        delete [] buffer;

        // The PATH has been loaded
        return true;
    }

    bool save()
    {
        // Normalize the paths (to native separators)
        for (QStringList::Iterator i = entries.begin(), end = entries.end(); i != end; i++)
            *i = QDir::toNativeSeparators(*i);

        // Convert back to a big fat string
        QString path = entries.join(';');

        // And again, try to forget there's such thing as type safety
        if (RegSetValueEx(registryKey, TEXT("Path"), 0, REG_SZ, reinterpret_cast<LPCBYTE>(path.utf16()), path.size() * sizeof(TCHAR) + 1) != ERROR_SUCCESS)  {
            qDaemonLog(QStringLiteral("Couldn't save the PATH registry key. You may need to update the system path manually."), QDaemonLog::WarningEntry);
            return false;
        }

        // Notify others of the change (don't catch errors as they aren't exactly reported with HWND_BROADCAST)
        SendMessageTimeout(HWND_BROADCAST, WM_SETTINGCHANGE, 0, (LPARAM) L"Environment", SMTO_ABORTIFHUNG, serviceNotifyTimeout, NULL);

        return true;
    }

    HKEY registryKey;
    QStringList entries;

    static LPCTSTR pathRegistryKey;
};

LPCTSTR WindowsSystemPath::pathRegistryKey = TEXT("SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment");

// -------------------------------------------------------------------------------------------------------------------------------------------------------- //
// --- ControllerBackendWindows --------------------------------------------------------------------------------------------------------------------------- //
// -------------------------------------------------------------------------------------------------------------------------------------------------------- //

ControllerBackendWindows::ControllerBackendWindows(QCommandLineParser & parser, bool autoQuit)
    : QAbstractControllerBackend(parser, autoQuit),
      updatePathOption(QStringLiteral("update-path"), QCoreApplication::translate("main", "Update the system PATH on install/uninstall."))
{
    parser.addOption(updatePathOption);
}

bool ControllerBackendWindows::start()
{
    WindowsServiceManager manager = WindowsServiceManager::open();
    if (!manager.isValid())
        return false;

    WindowsService service(QDaemonApplication::applicationName(), manager);
    if (!service.open(SERVICE_START | SERVICE_QUERY_STATUS) || !service.start())
        return false;

    QMetaObject::invokeMethod(qApp, "started", Qt::QueuedConnection);
    return true;
}

bool ControllerBackendWindows::stop()
{
    WindowsServiceManager manager = WindowsServiceManager::open();
    if (!manager.isValid())
        return false;

    WindowsService service(QDaemonApplication::applicationName(), manager);
    if (!service.open(SERVICE_STOP | SERVICE_QUERY_STATUS) || !service.stop())
        return false;

    QMetaObject::invokeMethod(qApp, "stopped", Qt::QueuedConnection);
    return true;
}

bool ControllerBackendWindows::install()
{
    WindowsServiceManager manager = WindowsServiceManager::open();
    if (!manager.isValid())
        return false;


    QStringList executable;
    executable << QDaemonApplication::applicationFilePath() << QStringLiteral("-d");

    QStringList arguments = parser.positionalArguments();
    if (arguments.size() > 0)
        executable << QStringLiteral("--") << arguments;

    WindowsService service(QDaemonApplication::applicationName(), manager);
    service.setDescription(QDaemonApplication::applicationDescription());
    service.setExecutable(executable.join(' '));

    if (!service.create())
        return false;

    if (parser.isSet(updatePathOption))  {
        WindowsSystemPath path;
        if (!path.addEntry(QCoreApplication::applicationDirPath()))
            return false;
    }

    QMetaObject::invokeMethod(qApp, "installed", Qt::QueuedConnection);
    return true;
}

bool ControllerBackendWindows::uninstall()
{
    WindowsServiceManager manager = WindowsServiceManager::open();
    if (!manager.isValid())
        return false;

    WindowsService service(QDaemonApplication::applicationName(), manager);
    if (!service.open(DELETE) || !service.remove())
        return false;

    if (parser.isSet(updatePathOption))  {
        WindowsSystemPath path;
        if (!path.removeEntry(QCoreApplication::applicationDirPath()))
            return false;
    }

    QMetaObject::invokeMethod(qApp, "uninstalled", Qt::QueuedConnection);
    return true;
}

QAbstractControllerBackend::DaemonStatus ControllerBackendWindows::status()
{
    WindowsServiceManager manager = WindowsServiceManager::open();
    if (!manager.isValid())
        return NotRunningStatus;

    WindowsService service(QDaemonApplication::applicationName(), manager);
    if (!service.open(SERVICE_QUERY_STATUS))
        return NotRunningStatus;

    switch (service.status())
    {
    case SERVICE_RUNNING:
        return RunningStatus;
    case SERVICE_STOPPED:
    default:
        return NotRunningStatus;
    }
}

QT_END_NAMESPACE
