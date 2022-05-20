#ifndef SERVICE_YA_H
#define SERVICE_YA_H

#include "../service.h"

class Service_Ya : public Service
{
  Q_OBJECT

private:
  //const QString API_token = "963b0794-b2f0-48eb-899f-7b142ed4c21f";
  QString get_service_name();
  QUrl request(const QString* q);
  coordinate reply(const QJsonDocument* doc);
};

#endif // SERVICE_YA_H
