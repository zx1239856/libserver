#ifndef PDFHANDLER_H
#define PDFHANDLER_H

#include <poppler-qt5.h>
#include <QImage>
#include <QDebug>

namespace pdf
{
  enum resolution{normal,low,mid,high,ultra};
}

class pdfHandler
{
public:
    explicit pdfHandler(const QString& filePath);
    ~pdfHandler();
    QImage getPdfImage(int pageNum, pdf::resolution res = pdf::normal)const;
    int getNumPages()const;
    QSize getPageSize()const;
private:
    Poppler::Document* document;
    QString filePath;
    int numPages;
    QSize pageSize;
    void getPdfInfo();
};

#endif // PDFHANDLER_H
