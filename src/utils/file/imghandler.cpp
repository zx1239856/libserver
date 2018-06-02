#include "imghandler.h"
#include <QDateTime>
#include <QString>

ImgHandler::ImgHandler(const QImage &_src):src(_src)
{
}

QImage ImgHandler::compress(compressRatio _ratio)
{
  int destWidth = src.width();
  int destHeight = src.height();
  qreal width = 0;
  bool bySize = false;
  switch(_ratio)
    {
    case webLow: width = 200;break;
    case webMid: width = 90;break;
    case webHigh: width = 30;break;
    case normalLow: width = 1000;break;
    case normalMid:width = 800;break;
    case normalHigh:width =500;break;
    default: bySize = true;
    }
  if(!bySize)
    {
      destHeight = width/ destWidth * destHeight;
      destWidth = width;
      return src.scaled(4*destWidth,4*destHeight,Qt::KeepAspectRatio).scaled(destWidth,destHeight,Qt::KeepAspectRatio,Qt::SmoothTransformation);
    }
  else
    {
      qreal rate = 0;
      switch(_ratio)
        {
        case bySizeLow: rate = 0.3;break;
        case bySizeMid: rate = 0.5;break;
        case bySizeHigh: rate = 0.8;break;
        default: break;
        }
      destHeight*=rate;
      destWidth*=rate;
      return src.scaled(destWidth,destHeight,Qt::KeepAspectRatio,Qt::SmoothTransformation);
    }
}

bool ImgHandler::save(const QImage& img, const QString &fileName,const QString& dir,int quality)
{
  return img.save(dir+fileName,nullptr,quality);
}

bool ImgHandler::save(const QImage& img, const QString& dir,int quality)
{
  qint64 time = QDateTime::currentMSecsSinceEpoch();
  if(img.hasAlphaChannel())
    {
        return img.save(dir+QString::number(time)+".PNG",nullptr,quality);
    }
  else return img.save(dir+QString::number(time)+".JPG",nullptr,quality);
}

bool ImgHandler::save(const QString &fileName,const QString& dir,int quality)
{
  return save(this->src,fileName,dir,quality);
}

bool ImgHandler::save(const QString& dir,int quality)
{
  return save(this->src,dir,quality);
}
