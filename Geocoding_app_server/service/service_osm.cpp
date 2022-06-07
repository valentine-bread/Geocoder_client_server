#include "service_osm.h"

QUrl Service_OSM::direct_request(const QString &q)  //запрос прямого геокодирования
{
  QUrl url("https://nominatim.openstreetmap.org/search");
  QUrlQuery query;
  query.addQueryItem("format","geocodejson");
  query.addQueryItem("limit","1");
  query.addQueryItem("q",q);
  url.setQuery(query.query());
  return url;
}

QUrl Service_OSM::reverse_request(const double &lat, const double &lon) //запрос обратного геокодирования
{
  QUrl url("https://nominatim.openstreetmap.org/reverse");
  QUrlQuery query;
  query.addQueryItem("lat",QString::number(lat));
  query.addQueryItem("lon",QString::number(lon));
  query.addQueryItem("format","geocodejson");
  query.addQueryItem("limit","1");
  url.setQuery(query.query());
  return url;
}

QGeoLocation Service_OSM::reply(const QJsonDocument* doc) //разбор ответа сервиса
{
  QGeoLocation loc;
  QGeoAddress addr;
  QJsonObject tmp;
  QJsonObject label_tmp;
  QJsonArray arr;
  //qDebug() << "OSM \t";
  if(doc->object().value("status").toInt() != 0){
      qDebug() << "OSM" << doc->object().value("message").toString();
    }
  arr = doc->object().value("features").toArray();
  if(arr.size() == 0){
      qDebug() << "OSM no result.";;
      return loc;
    }
  tmp = arr.at(0).toObject().value("geometry").toObject();
  label_tmp = arr.at(0).toObject().value("properties").toObject();
  label_tmp =  label_tmp.value("geocoding").toObject();
  addr.setText(label_tmp.value("label").toString());  //полный адрес
  arr = tmp.value("coordinates").toArray();
  loc.setAddress(addr);
  loc.setCoordinate(QGeoCoordinate(arr[1].toDouble(), arr[0].toDouble(),0)); // координаты
  return loc;
}

QString Service_OSM::get_service_name()
{
  return "OpenStreetMap";
}
