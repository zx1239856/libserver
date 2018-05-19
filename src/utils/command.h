/*#ifndef COMMAND_H
#define COMMAND_H

#include <QObject>

class command: public QObject
{
    Q_OBJECT
public:
    enum class CMD
    {
        USER,
        QUERY,
        APPOINT,
        OPERATEUSER,
        OPERATEGROUP,
        OPERATEBOOK,
        OPERATE,
        CHECK
    };

    enum class USER
    {
        login,
        forget,
        logout,
        changepwd,
        updateuserinfo
    };

    enum class QUERY
    {
        book,
        operationlog,
        borrowlog,
    };

    enum class APPOINT
    {
        appointborrow,
        appointreturn
    };

    enum class OPERATEUSER
    {
        createuser,
        deleteuser,
        changegroup
    };

    enum class OPERATEGROUP
    {
        createreader,
        createstaff,
        createbook,
        changereader,
        changestaff,
        changebook,
        deletereader,
        deletestaff,
        deletebook
    };
    enum class OPERATEBOOK
    {
        createbook,
        deletebook,
        changebook,
        changebookgroup
    };

    enum class OPERATE
    {
        changesetting
    };

    enum class CHECK
    {
        checkborrow,
        checkreturn
    };

    GetCmd(QString type, QString cmd)
    {

    }
};

#endif // COMMAND_H
*/
