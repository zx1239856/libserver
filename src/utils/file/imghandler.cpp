#include "imghandler.h"

ImgHandler::ImgHandler(const QImage &_src):src(_src)
{
}

QImage ImgHandler::compress(compressRatio _ratio)
{
  qreal ratio = 0;
  switch(_ratio)
    {
    case low: ratio = 0.8;break;
    case mid: ratio = 0.5;break;
    case high: ratio = 0.3;break;
    }
  auto destWidth = ratio*src.width();
  auto destHeight = ratio*src.height();
  return src.scaled(4*destWidth,4*destHeight,Qt::KeepAspectRatio).scaled(destWidth,destHeight,Qt::KeepAspectRatio,Qt::SmoothTransformation);
}
