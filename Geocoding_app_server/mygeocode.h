#ifndef MYGEOCODE_H
#define MYGEOCODE_H

#include "including.h"
#include "service/service_osm.h"
#include "service/service_mapbox.h"
#include "service/service_ya.h"

class My_Geocode : public QObject
{
  Q_OBJECT

public:
  explicit My_Geocode(QObject *parent = nullptr);
  ~My_Geocode();
  void geocoding_list(const QVariantMap&, const QVariantList&, const QString&);
  QVariantList get_all_service_name();
  QVariantMap get_all_API_key();
  void load_API_key();
  void download_API_key(const QVariantMap&);
  QTcpSocket* socket;


private:
  QList<Service*> service_list;
  unsigned int finish_flag;

private slots:
  void onfinish_geocoding_list(Service*);

signals:
  void finish_geocoding_all(QJsonDocument&);
};

#endif // MYGEOCODE_H
