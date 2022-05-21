#include "service_mapbox.h"

QString Service_MapBox::get_service_name()
{
  return "MapBox";
}

QUrl Service_MapBox::request(const QString* q)
{
  QUrl url("https://api.mapbox.com/geocoding/v5/mapbox.places/" + *q + ".json");
  QUrlQuery query;
  query.clear();
  query.addQueryItem("access_token", API_key);
  query.addQueryItem("limit","1");
  url.setQuery(query.query());
  return url;
}

QGeoLocation Service_MapBox::reply(const QJsonDocument* doc)
{
  QGeoLocation loc;
  QGeoAddress addr;
  QJsonObject tmp;
  QJsonArray arr;
  //qDebug() << "Mapbox \t";
  if(doc->object().value("message").toString() != ""){
      qDebug() << "MapBox" << doc->object().value("message").toString();
      return loc;
    }
  arr = doc->object().value("features").toArray();
  if(arr.size() == 0){
      qDebug() << "No result.";
      return loc;
    }
  tmp = arr.at(0).toObject().value("geometry").toObject();
  addr.setText(arr.at(0).toObject().value("place_name").toString());
  arr = tmp.value("coordinates").toArray();
  //qDebug() << get_service_name() << " " << label << arr[1].toDouble() << arr[0].toDouble();
  loc.setAddress(addr);
  loc.setCoordinate(QGeoCoordinate(arr[1].toDouble(),arr[0].toDouble(),0));
  return loc;

}
