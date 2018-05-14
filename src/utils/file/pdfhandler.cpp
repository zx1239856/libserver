#include "pdfhandler.h"

pdfHandler::pdfHandler(const QString& Path):
  document(nullptr),filePath(Path),numPages(0),pageSize(0,0)
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

QImage pdfHandler::getPdfImage(int pNum, pdf::resolution res)const
{
    using namespace pdf;
    QImage image;
    Poppler::Page* pdfPage = document->page(pNum);
    if (pdfPage == nullptr) {
        throw("Invalid page number specified");
        return image;
    }
    // Generate image from a certain page
    int dpi;
    switch(res)
      {
      case normal: dpi=72;break;
      case low: dpi=50;break;
      case mid: dpi=150;break;
      case high: dpi=300;break;
      case ultra: dpi=600;break;
      default: dpi=72;
      }
    image = pdfPage->renderToImage(dpi, dpi, -1, -1, -1, -1); // 72 dpi
    if (image.isNull()) {
        delete pdfPage;
        throw("Image conversion failed.");
        return image;
    }
    delete pdfPage;
    return image;
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
