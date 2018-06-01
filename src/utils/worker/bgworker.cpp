#include "bgworker.h"
#include "utils/file/pdfhandler.h"
#include "utils/file/imghandler.h"
#include <QDir>

AbstractWorker::AbstractWorker()
{}

AbstractWorker::~AbstractWorker()
{}

pdfConversion::pdfConversion(const QString &_src, const QString &_dst, mode Mode):src(_src),dst(_dst),currentMode(Mode),pdf(nullptr)
{}

pdfConversion::~pdfConversion()
{
  if(pdf)delete pdf;
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
  // set watermark here if possible
  //QImage img("HP.png");
  //pdf->setWatermark(img,45,0.5,2,true);
  //
  for(int i=0;i<pages;++i)
    {
      QImage &&img = pdf->getPdfImage(i,pdf::high);
      //ImgHandler handle(img);
      ImgHandler::save(img,fname.section(".",0,0)+"_"+QString::number(i+1)+".jpg",dst);
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
