#ifndef IMGHANDLER_H
#define IMGHANDLER_H

#include <QImage>
#include "../crypto/token.h"

class ImgHandler
{
private:
  const QImage &src;
public:
  // higher the ratio, lower the img quality
  // bySize* compress the img according to original size
  enum compressRatio
  {
    webLow,webMid,webHigh,normalLow,normalMid,normalHigh,bySizeLow,bySizeMid,bySizeHigh
  };
  ImgHandler(const QImage& src);
  QImage compress(compressRatio ratio);
  bool save(const QString &fileName,const QString& dir,int quality=-1);
  bool save(const QString& dir,int quality=-1);
  static bool save(const QImage& img, const QString &fileName,const QString& dir,int quality=-1);
  // default timestamp filename, auto detect alpha channel
  static bool save(const QImage& img, const QString& dir,int quality=-1);
};

#endif // IMGHANDLER_H
