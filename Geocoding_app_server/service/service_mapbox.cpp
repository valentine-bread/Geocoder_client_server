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

coordinate Service_MapBox::reply(const QJsonDocument* doc)
{
  QJsonObject tmp;
  QJsonArray arr;
  QString label = "No result";
  //qDebug() << "Mapbox \t";
  if(doc->object().value("message").toString() != ""){
      qDebug() << "MapBox" << doc->object().value("message").toString();
      return coordinate{label, -1, -1};
    }
  arr = doc->object().value("features").toArray();
  if(arr.size() == 0){
      qDebug() << "No result.";
      return coordinate{label, -1, -1};
    }
  tmp = arr.at(0).toObject().value("geometry").toObject();
  label = arr.at(0).toObject().value("place_name").toString();
  arr = tmp.value("coordinates").toArray();
  //qDebug() << arr[1].toDouble() << arr[0].toDouble();

  return coordinate{label, arr[1].toDouble(), arr[0].toDouble()};

}
