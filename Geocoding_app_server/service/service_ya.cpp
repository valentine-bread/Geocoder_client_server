#include "service_ya.h"

QUrl Service_Ya::request(const QString *q)
{
  QUrl url("https://geocode-maps.yandex.ru/1.x/");
  QUrlQuery query;
  query.clear();
  query.addQueryItem("apikey", API_key);
  query.addQueryItem("results","1");
  query.addQueryItem("format","json");
  query.addQueryItem("geocode",*q);
  url.setQuery(query.query());
  return url;
}

QGeoLocation Service_Ya::reply(const QJsonDocument* doc)
{
  QGeoLocation loc;
  QGeoAddress addr;
  QJsonObject tmp;
  QJsonArray arr;
  QJsonObject label_tmp;
  //qDebug() << "Ya";
  if(doc->object().value("statusCode").toInt() != 0){
      qDebug() <<"Ya" << doc->object().value("error").toString() << doc->object().value("message").toString();
      return loc;
    }
  tmp = doc->object().value("response").toObject();
  tmp = tmp.value("GeoObjectCollection").toObject();
  arr = tmp.value("featureMember").toArray();
  tmp = arr.at(0).toObject().value("GeoObject").toObject();
  label_tmp = tmp;
  label_tmp = label_tmp.value("metaDataProperty").toObject();
  label_tmp = label_tmp.value("GeocoderMetaData").toObject();
  addr.setText(label_tmp.value("text").toString());
  if(arr.size() == 0) {
      qDebug() << "Ya no result";
      return loc;
    }
  tmp = tmp.value("Point").toObject();
  QString result = tmp.value("pos").toString();
  QRegExp rx("(\\d+.\\d+) (\\d+.\\d+)");
  rx.indexIn(result);
  //qDebug() << get_service_name() << " " << label << rx.cap(2).toDouble() << " " << rx.cap(1).toDouble();
  loc.setAddress(addr);
  loc.setCoordinate(QGeoCoordinate(rx.cap(2).toDouble(), rx.cap(1).toDouble(),0));
  return loc;
}

QString Service_Ya::get_service_name()
{
  return "YandexMap";
}
