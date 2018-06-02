#include "bgworker.h"
#include "utils/file/pdfhandler.h"
#include "utils/file/imghandler.h"
#include "utils/dbwrapper/db-operation.h"
#include "globalInfo.h"
#include <QDir>

AbstractWorker::AbstractWorker()
{}

AbstractWorker::~AbstractWorker()
{}

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
  using namespace globalInfo;
  // get watermark filename
  sql::select sel(dbFullPrefix+"settings","variable='pdfWaterMark'",{"value"});
  if(sel.exec())
    {
      auto res = sel.toResult();
      if(!res.empty())
        {
          watermk = res[0].value("value").toString();
        }
      else
        {
          qDebug() << "Watermark settings not present in db";
        }
    }
  else
    {
      qDebug() << "Failed to fetch watermark filename from db";
    }
  if(!watermk.isEmpty())
    {
      watermkImg = new QImage(config::getInstance()->dataDir()+"watermark/"+watermk);
    }
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
