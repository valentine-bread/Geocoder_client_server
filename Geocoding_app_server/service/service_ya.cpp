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

coordinate Service_Ya::reply(const QJsonDocument* doc)
{
  QJsonObject tmp;
  QJsonArray arr;
  QJsonObject label_tmp;
  QString label = "No result";
  //qDebug() << "Ya";
  if(doc->object().value("statusCode").toInt() != 0){
      qDebug() <<"Ya" << doc->object().value("error").toString() << doc->object().value("message").toString();
      return coordinate{label, -1, -1};
    }
  tmp = doc->object().value("response").toObject();
  tmp = tmp.value("GeoObjectCollection").toObject();
  arr = tmp.value("featureMember").toArray();
  tmp = arr.at(0).toObject().value("GeoObject").toObject();
  label_tmp = tmp;
  label_tmp = label_tmp.value("metaDataProperty").toObject();
  label_tmp = label_tmp.value("GeocoderMetaData").toObject();
  label = label_tmp.value("text").toString();
  if(arr.size() == 0) {
      qDebug() << "Ya no result";
      return coordinate{label, -1, -1};
    }
  tmp = tmp.value("Point").toObject();
  QString result = tmp.value("pos").toString();
  QRegExp rx("(\\d+.\\d+) (\\d+.\\d+)");
  rx.indexIn(result);
  //qDebug() << rx.cap(2).toDouble() << " " << rx.cap(1).toDouble();

  return coordinate{label, rx.cap(2).toDouble(), rx.cap(1).toDouble()};
}

QString Service_Ya::get_service_name()
{
  return "YandexMap";
}
