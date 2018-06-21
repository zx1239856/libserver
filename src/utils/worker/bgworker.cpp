#include "bgworker.h"
#include "utils/file/pdfhandler.h"
#include "utils/file/imghandler.h"
#include "utils/handle/dbsettings.h"
#include "utils/config.h"
#include "utils/dbwrapper/db-operation.h"
#include "globalInfo.h"
#include "utils/handle/handle.h"
#include "utils/smtp/sendemail.h"
#include <QDir>

AbstractWorker::AbstractWorker()
{}

AbstractWorker::~AbstractWorker()
{}

#ifdef Q_OS_LINUX
pdfConversion::pdfConversion(const QString &_src, const QString &_dst, mode Mode,uint dp):src(_src),dst(_dst),currentMode(Mode),pdf(nullptr)
{
  dpi = dp;
}

pdfConversion::~pdfConversion()
{
  if(pdf)delete pdf;
  if(watermkImg)delete watermkImg;
}

bool pdfConversion::process(const QString file)
{
  QFileInfo info(file);
  QString fname = info.fileName();
  int pages = 0;
  if(pdf)delete pdf;
  pdf = new pdfHandler(file);
  pages = pdf->getNumPages();
  if(pages == 0)
    {
      return false;
    }
  watermk = dbSettings::getSetting("pdfWaterMark");
  if(!watermk.isEmpty() && !watermkImg)
    {
      watermkImg = new QImage(config::getInstance()->dataDir()+"watermark/"+watermk);
    }
  if(watermkImg)
  {
     pdf->setWatermark(*watermkImg,45,0.2,1);
  }
  for(int i=0;i<pages;++i)
    {
      QImage &&img = pdf->getPdfImage(i,dpi);
      img.save(dst+fname.section(".",0,0)+"_"+QString::number(i+1)+".jpg",nullptr,62);
    }
  if(pdf)
    {
      delete pdf;
      pdf = nullptr;
    }
  return true;
}

void pdfConversion::run()
{
  if(currentMode == singleFile)
    {
      if(!process(src))
        {
          emit onFail("Empty or invalid PDF, ignore this file");
          return;
        }
      else emit onFinish();
    }
  else if(currentMode == multiFile)
    {
      // process file within the directory
      QDir dir(src);
      QStringList nameFilters;
      nameFilters << "*.pdf" << "*.PDF";
      QStringList files = dir.entryList(nameFilters,QDir::Files|QDir::Readable,QDir::Name);
      for(auto it=files.begin();it!=files.end();++it)
        {
          if(!process(src + (*it)))
            {
              emit onFail("Empty or invalid PDF, ignore this file");
            }
        }
      emit onFinish();
    }
}
#endif

void borrowNotifier::run()
{
  while(true)
  {
    sql::select sel(globalInfo::dbFullPrefix+"currborrow","DATEDIFF(CURRENT_TIMESTAMP,exptime)<="+QString::number(3));
    sel.addLimit(50,currMax);
    if(sel.exec())
    {
      QVector<QSqlRecord> res = sel.toResult();
      if(res.isEmpty())break;
      for(const QSqlRecord x:res)
      {
        ++currMax;
        QString expTime = x.value("exptime").toDateTime().toString("yyyy-MM-dd");
        QString reader = x.value("readerid").toString();
        QString book = x.value("bookid").toString();
        QString readerEmail;
        QString bookName;
        sql::select sel2(globalInfo::dbFullPrefix+"readers","ID="+reader,{"email"});
        if(sel2.exec())
        {
           auto r = sel2.toResult();
           if(!r.isEmpty())readerEmail = r[0].value("email").toString();
        }
        sql::select sel3(globalInfo::dbFullPrefix+"books","ID="+book,{"name"});
        if(sel3.exec())
        {
           auto r = sel3.toResult();
           if(!r.isEmpty())bookName = r[0].value("name").toString();
        }
        // echo the name of the book & reader's email
        qDebug()<<readerEmail<<" "<<bookName << " " << expTime;
        // send email
        QString emailContent = globalInfo::emailNotifyContent;
        // find email template
        QString tplPath = config::getInstance()->templateDir() + "/returnNotify.html";
        QFileInfo tpl(tplPath);
        if(tpl.isFile())
        {
            QFile file(tplPath);
            if (file.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                QTextStream in(&file);
                emailContent = in.readAll();
                emailContent.replace("%%Servicename",globalInfo::appName);
            }
        }
        emailContent.replace("%%Bookname",bookName);
        emailContent.replace("%%Exptime",expTime);
        QStringList rcp;
        rcp << readerEmail;
        sendEmail email(*config::getInstance(), rcp, globalInfo::emailNotifyTitle, emailContent);
        QObject::connect(&email,&sendEmail::onFail,this,[&](const QString& what)
        {
            qDaemonLog(what,QDaemonLog::ErrorEntry);
        });
        email.send();
      }
    }
    else
    {
      logDbErr(&sel);  // abort while occuring error
      break;
    }
  }
  currMax = 0;
}

borrowNotifier::~borrowNotifier()
{}
