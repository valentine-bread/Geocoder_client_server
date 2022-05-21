#ifndef SERVICE_Mapbox_H
#define SERVICE_Mapbox_H

#include "../service.h"

class Service_MapBox : public Service
{
  Q_OBJECT

private:
  QString get_service_name();
  //const QString API_token = "pk.eyJ1IjoidmFsZW50aW5ldiIsImEiOiJja3k1MWx4OGMwaGJ6MndwdmY5YzFuMXp2In0.5N_zjKOJw3tF4HXpZ6U8hg";
  QUrl request(const QString* q);
  QGeoLocation reply(const QJsonDocument* doc);
};

#endif
