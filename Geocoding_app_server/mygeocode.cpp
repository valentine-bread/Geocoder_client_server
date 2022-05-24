#include "mygeocode.h"

My_Geocode::My_Geocode(QObject *parent)
  : QObject{parent}
{
  service_list.push_back(new Service_OSM);//Добавление сервиса OSM
  service_list.push_back(new Service_MapBox);
  service_list.push_back(new Service_Ya);

  for(auto &it : service_list){ //Обрадение к списку сервисов
      connect(it,SIGNAL(finish_geocoding_list(Service*)),this,SLOT(onfinish_geocoding_list(Service*)));
    }
  socket = new QTcpSocket(this);
  qDebug() << "Ok";
}

My_Geocode::~My_Geocode()
{
  for(auto &it : service_list)
    delete it;
  delete socket;
}

QVariantList My_Geocode::get_all_service_name()
{
  QVariantList list;
  for(auto &it : service_list){
      list.append(it->get_service_name());
    }
  return list;
}

QVariantMap My_Geocode::get_all_API_key()
{
  QVariantMap list;
  for(auto &it : service_list){
      list.insert(it->get_service_name(),it->get_API_key());
    }
  return list;
}

void My_Geocode::load_API_key()
{
  QFile f("./key.json");

  if(!f.open(QFile::ReadOnly | QIODevice::Text)) return;
  //QTextStream readStream(&f);
  QJsonParseError docErr;
  QJsonDocument doc = QJsonDocument::fromJson(f.readAll(),&docErr);
  if (docErr.error != QJsonParseError::NoError){
      qDebug() << "key.json: json parse error";
      return;
    }
  for(auto &it : service_list){
      it->set_API_key(doc.object().value(it->get_service_name()).toString());
      qDebug() << doc.object().value(it->get_service_name()).toString();
    }
  f.close();
}

void My_Geocode::download_API_key(const QVariantMap &key_map)
{
  QFile jsonFile("./key.json");
  jsonFile.open(QFile::WriteOnly);
  QJsonObject obj;
  QJsonDocument doc;
  for(auto &it : service_list){
      qDebug() <<  key_map.value(it->get_service_name()).toString();
      obj.insert(it->get_service_name(),key_map.value(it->get_service_name()).toString());
    }
  doc.setObject(obj);
  jsonFile.write(doc.toJson());
  //qDebug() << obj;
  jsonFile.close();
}

void My_Geocode::onfinish_geocoding_list(Service* s)
{
  qDebug() << s->get_service_name() << " " << s->get_time() << "ms";
  finish_flag--;
  if(finish_flag == 0){
      QJsonObject rez_obj;
      for(auto &it : service_list){
          QJsonArray arr;
          auto rez = it->get_rez_data();
          for(auto &x : *rez){
              QJsonObject obj;
              if(!x.location.isEmpty()){
                obj.insert("address", x.location.address().text());
                obj.insert("lat", x.location.coordinate().latitude());
                obj.insert("lon", x.location.coordinate().longitude());
                }
              else{
                  obj.insert("address", "No result");
                }
              arr.push_back(obj);
            }
          rez_obj.insert(it->get_service_name(), arr);
        }
      qDebug() << "finish geocoding" << rez_obj;
      rez_obj.insert("type",1);
      QJsonDocument doc(rez_obj);
      emit finish_geocoding_all(doc);
    }
}


void My_Geocode::geocoding_list(const QVariantMap &check_map, const QVariantList& address_list,const QString& type)
{
  finish_flag = 0;
  for(auto &it : service_list) {
      if(check_map.value(it->get_service_name()).toBool() == true){
        finish_flag++;
        it->geocoding_list(address_list, type);
        }
    }
}
