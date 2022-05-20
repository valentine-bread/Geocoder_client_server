#include "service_osm.h"

QUrl Service_OSM::request(const QString *q)
{
  QUrl url("https://nominatim.openstreetmap.org/search");
  QUrlQuery query;
  query.addQueryItem("format","geocodejson");
  query.addQueryItem("limit","1");
  query.addQueryItem("q",*q);
  url.setQuery(query.query());
  return url;
}

coordinate Service_OSM::reply(const QJsonDocument* doc)
{
  QJsonObject tmp;
  QJsonObject label_tmp;
  QJsonArray arr;
  QString label = "No result";
  //qDebug() << "OSM \t";
  if(doc->object().value("status").toInt() != 0){
      qDebug() << "OSM" << doc->object().value("message").toString();
    }
  arr = doc->object().value("features").toArray();
  if(arr.size() == 0){
      qDebug() << "OSM no result.";;
      return coordinate{label, -1, -1};
    }
  tmp = arr.at(0).toObject().value("geometry").toObject();
  label_tmp = arr.at(0).toObject().value("properties").toObject();
  label_tmp =  label_tmp.value("geocoding").toObject();
  label = label_tmp.value("label").toString();
  arr = tmp.value("coordinates").toArray();
  //qDebug() << arr[1].toDouble() << " " << arr[0].toDouble();
  return coordinate{label, arr[1].toDouble(), arr[0].toDouble()};
}

QString Service_OSM::get_service_name()
{
  return "OpenStreetMap";
}
