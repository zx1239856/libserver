#ifndef MAINSERVICE_H
#define MAINSERVICE_H

#include <QObject>

class mainService : public QObject
{
  Q_OBJECT
public:
  explicit mainService(QObject *parent = nullptr);

signals:

public slots:
};

#endif // MAINSERVICE_H