// Qt lib import
#include <QCoreApplication>

#include "utils/dbwrapper/db-wrapper.h"
#include "utils/dbwrapper/db-operation.h"
#include "utils/config.h"

// global variables
static QTextStream cout(stdout, QIODevice::WriteOnly);
// global configuration file
config globalConf("./config.conf");

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    std::vector<QString> fields{"id","typename","privilege"};
    std::vector<QVariantList> vals{{1,2,3},{"Admin","Stock","Test"},{false,false,false}};
    sql::insert in("lib_settings.stafftype",fields,vals);
    in.exec();
 /*
    auto insert = [&]() {
        auto query(control.query()); // 这里的query在解引用（ -> 或者 * ）后返回的是 QSqlQuery ，直接用就可以了，不需要单独打开数据库或者其他的初始化

        query->prepare("INSERT INTO Persons (id, City) VALUES (?, 123)"); // 模拟插入操作

        query->addBindValue(rand() % 1280);

        if (!query->exec())
        {
            qDebug() << "Error" << __LINE__;
        }

        query->clear();
    };
    auto delete_ = [&]() {
        auto query(control.query());

        query->prepare("delete from Persons where id = ?");

        query->addBindValue(rand() % 1280);

        if (!query->exec())
        {
            qDebug() << "Error" << __LINE__;
        }
        query->clear();
    };
    auto update = [&]() {
        auto query(control.query());

        query->prepare("update Persons set City = 1111 where id = ?");

        query->addBindValue(rand() % 1280);
        query->addBindValue(rand() % 1280);

        if (!query->exec())
        {
            qDebug() << "Error" << __LINE__;
        }

        query->clear();
    };
    auto select = [&]() {
        auto query(control.query());

        query->prepare("select * from Persons where id = ?");

        query->addBindValue(rand() % 1280);

        if (!query->exec())
        {
            qDebug() << "Error" << __LINE__;
        }

        query->clear();

        query->prepare("select * from Test2 where data1 = ?");

        query->addBindValue(rand() % 1280);

        if (!query->exec())
        {
            qDebug() << "Error" << __LINE__;
        }
    };*/

    /*QThreadPool::globalInstance()->setMaxThreadCount(10);

    for (int now = 0; now < 1; now++) // 开启3个线程测试
    {
        QtConcurrent::run([&]() {
            while (true)
            {
                switch (rand() % 20)
                {
                case 0:
                {
                    insert();
                    break;
                }
                case 1:
                {
                    delete_();
                    break;
                }
                case 2:
                {
                    update();
                    break;
                }
                default:
                {
                    select();
                    break;
                }
                }
            }
            QThread::msleep(10);
        });
    }*/

    return a.exec();
}
