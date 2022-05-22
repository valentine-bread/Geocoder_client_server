#ifndef MYCORE_H
#define MYCORE_H

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonParseError>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDebug>
#include <QUrlQuery>
#include <QFile>
#include <QTime>
#include <QTcpSocket>
#include <QTimer>
//#include "service/service_osm.h"
//#include "service/service_mapbox.h"
//#include "service/service_ya.h"

class My_Geocode : public QObject
{
  Q_OBJECT
  Q_PROPERTY(QString Geocode READ Geocode WRITE setGeocode NOTIFY GeocodeChanged)
public:
  enum Command{test, geocoding_address_list, size, set_service_key, get_service_key};
  explicit My_Geocode(QObject *parent = nullptr);
  ~My_Geocode();
  QString Geocode();
  Q_INVOKABLE void setGeocode(QString);
  //Q_INVOKABLE void geocoding(const QString);
  Q_INVOKABLE void direct_geocoding_list(const QVariantMap&);
  Q_INVOKABLE void reverse_geocoding_list(const QVariantMap&);
  //Q_INVOKABLE void geocoding_test();
  Q_INVOKABLE QVariantList load_in_file(const QString);
  Q_INVOKABLE void get_rez(const QUrl&);
  Q_INVOKABLE void set_address_list(QVariantList);
  Q_INVOKABLE QVariantList get_all_service_name();
  Q_INVOKABLE QVariantMap get_all_API_key();
  Q_INVOKABLE void load_API_key(const QVariantMap&);
  Q_INVOKABLE void download_API_key();

private:
  QTcpSocket* socket;
  QTimer* timer;
  QVariantList service_list;
  QVariantMap service_API_key;
  //QList<Service*> service_list;
  QVariantList* address_mas;
  QMap<QString, QVariantList>* result;
  int message_size = 0;

private slots:
  //void onfinish_geocoding_list(Service*);
  void onDisconnected();
  void onReadyRead();
  void connection();

signals:
  void GeocodeChanged();
  //void getcode(QString name, QString address, double lat, double lon);
  void getcode(QString name,QVariantList address);
  void finish_download_key();

};

#endif // MYCORE_H
