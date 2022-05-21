#ifndef SERVICE_OSM_H
#define SERVICE_OSM_H

#include "../service.h"

class Service_OSM : public Service
{
  Q_OBJECT

private:
  QString get_service_name();
  QUrl request(const QString* q);
  QGeoLocation reply(const QJsonDocument* doc);

};

#endif // SERVICE_OSM_H
