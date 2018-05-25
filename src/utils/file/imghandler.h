#ifndef IMGHANDLER_H
#define IMGHANDLER_H

#include <QImage>
#include "../cryto/token.h"

class ImgHandler
{
private:
  const QImage &src;
public:
  enum compressRatio
  {
    low,mid,high
  };
  ImgHandler(const QImage& src);
  QImage compress(compressRatio ratio);
  bool save(const QString &fileName,const QString& dir);
  // default MD5 filename
  bool save(const QString& dir);
};

#endif // IMGHANDLER_H
