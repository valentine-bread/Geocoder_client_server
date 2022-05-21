#include "mygeocode.h"

My_Geocode::My_Geocode(QObject *parent)
  : QObject{parent}
{
  service_list.push_back(new Service_OSM);
  service_list.push_back(new Service_MapBox);
  service_list.push_back(new Service_Ya);

  for(auto &it : service_list){
      connect(it,SIGNAL(finish_geocoding_list(Service*)),this,SLOT(onfinish_geocoding_list(Service*)));
      //qDebug() << it->get_service_name();
    }
  socket = new QTcpSocket(this);
  //address_mas = new QVariantList();
  qDebug() << "Ok";
}

My_Geocode::~My_Geocode()
{
  for(auto &it : service_list)
    delete it;
  delete socket;
}

//void My_Geocode::setGeocode(QString){}

/*void My_Geocode::geocoding_test()
{
  emit getcode("OpenStreetMap", "", 55.7504, 37.6175);
  emit getcode("MapBox", "",55.7558, 37.6178);
  emit getcode("YandexMap",  "", 55.7532, 37.6225);
}*/

/*QVariantList My_Geocode::load_in_file(QString path)
{
  if (path.isEmpty()) return *address_mas;
  path.remove(0,8);
  QFile f(path);
  if(!f.open(QFile::ReadOnly | QIODevice::Text)) return *address_mas;
  address_mas->clear();
  while(!f.atEnd()){
      auto line = f.readLine();
      address_mas->push_back(line);
    }
  f.close();
  qDebug() << "rez";
  for(auto &x : *address_mas){
      qDebug() << x.toString();
    }
  f.close();
  return *address_mas;
}*/

/*void My_Geocode::set_address_list(QVariantList list)
{
  delete address_mas;
  address_mas = new QVariantList(list);
  for(auto &x : *address_mas)
     qDebug() << x.toString();
}*/

/*void My_Geocode::get_rez(const QUrl &path)
{
  QFile f(path.fileName());

  if(!f.open(QFile::WriteOnly | QIODevice::Text)) return;
  QTextStream writeStream(&f);
  for(auto &it : service_list){
      auto rez = it->get_rez_data();
      for(auto &x : *rez){
          writeStream << x.address << " " << x.lat << " " << x.lon << "\n";
        }
      writeStream  << "\n";
    }
  f.close();
}*/

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
  qDebug() << finish_flag;
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
      QJsonDocument doc(rez_obj);
      emit finish_geocoding_all(doc);
    }
}

/*void My_Geocode::geocoding(QString q)
{
  //for(auto &it : service_list) it->geocoding_list(address_mas);
}*/

void My_Geocode::geocoding_list(const QVariantMap &check_map, const QVariantList& address_list)
{
  finish_flag = 0;
  for(auto &it : service_list) {
      if(check_map.value(it->get_service_name()).toBool() == true){
        finish_flag++;
        it->geocoding_list(address_list);
        }
    }
}
