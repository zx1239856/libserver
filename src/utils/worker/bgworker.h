#ifndef BGWORKER_H
#define BGWORKER_H

#include <QObject>

/* Copyright 2018 Zhang Xiang
 * zx1239856@gmail.com
 * MIT License
 * Description: worker class used to run crontab tasks
 */

class AbstractWorker:public QObject
{
  Q_OBJECT
public:
  AbstractWorker();
  virtual void run()=0;
  virtual ~AbstractWorker();
signals:
  void onFinish();
  void onFail(const QString &err);
};

class pdfHandler;

class pdfConversion: public AbstractWorker
{
  Q_OBJECT
public:
  enum mode
  {
    singleFile,
    multiFile
  };
private:
  QString watermk;
  QImage *watermkImg = nullptr;
  const QString src;
  const QString dst;
  mode currentMode;
  pdfHandler *pdf;
  uint dpi;
  bool process(const QString file);
public:
  pdfConversion(const QString &src, const QString &dst, mode Mode,uint dp=200);
  void run() override;
  ~pdfConversion()override;
};

#endif // BGWORKER_H
