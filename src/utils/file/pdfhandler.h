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
    void setWatermark(const QImage &watermark,qreal rot=0, qreal op=0.5, int l=1, bool full=true);
private:
    Poppler::Document* document;
    QString filePath;
    // watermark parameter
    const QImage *waterMark;
    qreal rotation;
    qreal opacity;
    int line;
    bool full;
    // pdf document parameter
    int numPages;
    QSize pageSize;
    void getPdfInfo();
    bool addWaterMark(QImage &src, const QImage &watermark,qreal rotation, qreal opacity, int line, bool full)const;
};

#endif // PDFHANDLER_H
