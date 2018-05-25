#ifndef SENDEMAIL_H
#define SENDEMAIL_H

#include "utils/smtp/SmtpMime"
#include "utils/config.h"

class sendEmail:public QThread
{
  Q_OBJECT
private:
  // basic items needed to connect SMTP server
  QString smtpServer;
  int port;
  SmtpClient::ConnectionType authMethod;
  QString user;
  QString password;
  EmailAddress *sender;
  // other parameters
  const QStringList& rcptList;
  const QString &subject;
  const QString &content;
  const QStringList& attachmentList;
public:
  sendEmail(const config &conf,const QStringList& rcptList,
            const QString &subject, const QString &content,const QStringList& attachmentList=QStringList());
  void run()override;
signals:
  void onSuccess();
  void onFail(const QString &what);
};

#endif // SENDEMAIL_H
