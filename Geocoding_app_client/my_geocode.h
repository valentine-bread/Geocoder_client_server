#ifndef MY_GEOCODE_H
#define MY_GEOCODE_H

#include "including.h"
//#include "service/service_osm.h"
//#include "service/service_mapbox.h"
//#include "service/service_ya.h"

class My_Geocode : public QObject
{
  Q_OBJECT
  Q_PROPERTY(QString Geocode READ Geocode WRITE setGeocode NOTIFY GeocodeChanged)
public:
  enum Command{test, geocoding_addres_list, size, service_key};
  explicit My_Geocode(QObject *parent = nullptr);
  ~My_Geocode();
  QString Geocode();
  Q_INVOKABLE void setGeocode(QString);
  //Q_INVOKABLE void geocoding(const QString);
  Q_INVOKABLE void geocoding_list(const QVariantMap&);
  //Q_INVOKABLE void geocoding_test();
  Q_INVOKABLE QVariantList load_in_file(const QString);
  Q_INVOKABLE void get_rez(const QUrl&);
  Q_INVOKABLE void set_addres_list(QVariantList);
  Q_INVOKABLE QVariantList get_all_service_name();
  Q_INVOKABLE QVariantMap get_all_API_key();
  void load_API_key();
  Q_INVOKABLE void download_API_key();



private:
  QTcpSocket* socket;
  QTimer* timer;
  QVariantList service_list;
  QVariantMap service_API_key;
  //QList<Service*> service_list;
  QVariantList* addres_mas;
  int message_size = 0;

private slots:
  //void onfinish_geocoding_list(Service*);
  void onDisconnected();
  void onReadyRead();
  void connection();

signals:
  void GeocodeChanged();
  void getcode(QString name, QString addres, double lat, double lon);
  void finish_download_key();

};

#endif // MY_GEOCODE_H
