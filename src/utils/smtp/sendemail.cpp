#include "sendemail.h"

sendEmail::sendEmail(const config &_conf,const QStringList& _rcptList,
                     const QString &_subject, const QString &_content,const QStringList& _attachmentList):
  smtpServer(_conf.smtpServer()),port(_conf.smtpPort()),authMethod(SmtpClient::TcpConnection),user(_conf.smtpUser()),
  password(_conf.smtpPwd()),sender(new EmailAddress(_conf.smtpSender())),rcptList(_rcptList),subject(_subject),
  content(_content),attachmentList(_attachmentList)
{
  QString auth = _conf.authType();
  if(auth=="SSL")
    {
      authMethod = SmtpClient::SslConnection;
    }
  else if(auth=="TLS")
    {
      authMethod = SmtpClient::TlsConnection;
    }
  else
    {
      authMethod = SmtpClient::TcpConnection;
    }
}

void sendEmail::send()
{
  SmtpClient smtp(smtpServer,port,authMethod);
  MimeMessage message;
  message.setSender(sender);
  message.setSubject(subject);
  MimeHtml html;
  html.setHtml(content);
  message.addPart(&html);
  for(auto it = rcptList.begin();it!=rcptList.end();++it)
    {
      message.addRecipient(new EmailAddress(*it));
    }
  for(auto it = attachmentList.begin();it!=attachmentList.end();++it)
    {
      message.addPart(new MimeAttachment(new QFile(*it)));
    }
  // email construction OK
  if (!smtp.connectToHost())
     {
         emit onFail(QString("SMTP connection Failed"));
         return;
     }

  if (!smtp.login(user, password))
  {
      emit onFail(QString("SMTP authentification Failed"));
      return;
  }

  if (!smtp.sendMail(message))
  {
      emit onFail(QString("Mail sending failed"));
      return;
  }
  else
  {
      smtp.quit();
      emit onSuccess();
      return;
  }
}
