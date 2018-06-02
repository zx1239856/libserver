#include "pdfhandler.h"
#include <QPainter>
#include <QtMath>

pdfHandler::pdfHandler(const QString& Path):
  document(nullptr),filePath(Path),waterMark(nullptr),rotation(0),opacity(0.5),line(1),full(true),numPages(0),pageSize(0,0)
{
    document = Poppler::Document::load(filePath);
    if (!document || document->isLocked()) {
      delete document;
      throw("Unable to open document.");
    }
    getPdfInfo();
}

pdfHandler::~pdfHandler()
{
  if(document)delete document;
}

QImage pdfHandler::getPdfImage(int pNum, uint dpi)const
{
  using namespace pdf;
  QImage image;
  Poppler::Page* pdfPage = document->page(pNum);
  if (pdfPage == nullptr) {
      throw("Invalid page number specified");
      return image;
  }
  // Generate image from a certain page
  image = pdfPage->renderToImage(dpi, dpi, -1, -1, -1, -1);
  if(waterMark)
    {
       addWaterMark(image,*waterMark,rotation,opacity,line,full);
    }
  if (image.isNull()) {
      delete pdfPage;
      throw("Image conversion failed.");
      return image;
  }
  delete pdfPage;
  return image;
}

QImage pdfHandler::getPdfImage(int pNum, pdf::resolution res)const
{
  using namespace pdf;
  uint dpi;
  switch(res)
    {
    case normal: dpi=72;break;
    case low: dpi=50;break;
    case mid: dpi=150;break;
    case good: dpi=225;break;
    case high: dpi=300;break;
    case ultra: dpi=600;break;
    default: dpi=72;
    }
  return getPdfImage(pNum,dpi);
}

int pdfHandler::getNumPages()const
{
    return numPages;
}
QSize pdfHandler::getPageSize()const
{
    return pageSize;
}

void pdfHandler::getPdfInfo() {
    numPages = document->numPages();
    Poppler::Page* pdfPage = document->page(0);
    if(!pdfPage)throw("Error parsing pages.");
    pageSize = pdfPage->pageSize();
    delete pdfPage;
}

bool pdfHandler::addWaterMark(QImage &src,const QImage &watermark,qreal rotation, qreal opacity, int line, bool full)const
{
  if(!watermark.hasAlphaChannel())
    {
      qWarning() << "Watermark does not have alpha channel. Please use watermark with alpha channel.";
      return false;
    }
  if(line<0)
    {
      qWarning() << "Invalid watermark number.";
      return false;
    }
  if(opacity<0||opacity>1)
    {
      qWarning() << "Invalid opacity number.";
      return false;
    }
  rotation=-rotation;
  QPainter painter(&src);
  int ylines = line;
  qreal yoffset = static_cast<qreal>(src.height())/(ylines);
  // resize the watermark to fit with the bg
  qreal ratio=0.8*std::min(src.height(),src.width())/std::max(watermark.height(),watermark.width());
  QImage water = watermark.scaledToHeight(watermark.height()*ratio/ylines);
  water = water.scaledToWidth(watermark.width()*ratio/ylines);
  // set opacity
  painter.setOpacity(opacity);
  int xlines = src.width()*1.0*ylines/src.height();
  qreal xoffset = src.width()/(xlines+1);
  // start to draw watermark
  for(int i=0;i<ylines;++i)
    {
      painter.translate(0,yoffset*(i+1)-yoffset/2);
      if(ylines==1)
        {
          painter.translate(src.width()/2,0);
          painter.rotate(rotation);
          painter.drawImage(-0.5*water.width(),-0.5*water.height(),water);
        }
      else
        {
          for(int j=0;j<=xlines+1;++j)
            {
              bool condition;
              if(!full)condition=!(i%2)^(j%2);
              else condition=true;
              if(condition)
                {
                  painter.rotate(rotation);
                  painter.drawImage(-0.5*water.width(),-0.5*water.height(),water);
                  painter.rotate(-rotation);
                }
              painter.translate(xoffset,0);
            }
        }
      painter.resetTransform();
    }
  return true;
}

void pdfHandler::setWatermark(const QImage &water,qreal rot, qreal op, int l,bool f)
{
  waterMark=&water;
  rotation=rot;
  opacity=op;
  line=l;
  full=f;
}
