#ifndef MY_GEOCODE_H
#define MY_GEOCODE_H

#include "including.h"
#include "service/service_osm.h"
#include "service/service_mapbox.h"
#include "service/service_ya.h"

class My_Geocode : public QObject
{
  Q_OBJECT
  //Q_PROPERTY(QString Geocode READ Geocode WRITE setGeocode NOTIFY GeocodeChanged)
public:
  explicit My_Geocode(QObject *parent = nullptr);
  ~My_Geocode();
  //QString Geocode();
  //void setGeocode(QString);
  //void geocoding(const QString);
  void geocoding_list(const QVariantMap&, const QVariantList&);
  //void geocoding_test();
  //QVariantList load_in_file(const QString);
  //void set_addres_list(QVariantList);
  //void get_rez(const QUrl&);
  QVariantList get_all_service_name();
  QVariantMap get_all_API_key();
  void load_API_key();
  void download_API_key(const QVariantMap&);
  QTcpSocket* socket;


private:
  QList<Service*> service_list;
  unsigned int finish_flag;
  //QVariantList* addres_mas;

private slots:
  void onfinish_geocoding_list(Service*);

signals:
  //void GeocodeChanged();
  //void getcode(QString name, QString addres, double lat, double lon);
  void finish_geocoding_all(QJsonDocument&);
};

#endif // MY_GEOCODE_H
