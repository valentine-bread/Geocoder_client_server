#ifndef SERVICE_OSM_H
#define SERVICE_OSM_H

#include "../service.h"

class Service_OSM : public Service
{
  Q_OBJECT

private:
  QString get_service_name();
  QUrl direct_request(const QString &q);
  QUrl reverse_request(const double &lat, const double &lon);
  QGeoLocation reply(const QJsonDocument* doc);

};

#endif // SERVICE_OSM_H
