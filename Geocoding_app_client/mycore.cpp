#include "mycore.h"

My_Geocode::My_Geocode(QObject *parent)
  : QObject{parent}
{
  //service_list.push_back(new Service_OSM);
  //service_list.push_back(new Service_MapBox);
  //service_list.push_back(new Service_Ya);

  /*for(auto &it : service_list){
      connect(it,SIGNAL(finish_geocoding_list(Service*)),this,SLOT(onfinish_geocoding_list(Service*)));
      //qDebug() << it->get_service_name();
    }*/
  socket = new QTcpSocket(this);
  connect(socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));  //сигналы от сокета
  connect(socket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));

  timer = new QTimer; //таймер для поключения
  timer->setInterval(500);
  connect(timer,SIGNAL(timeout()),this,SLOT(connection()));
  timer->start();
  address_mas = new QVariantList();
  result = new QMap<QString,QVariantList>();
  qDebug() << "Ok";  
}

My_Geocode::~My_Geocode()
{
  //for(auto &it : service_list)
    //delete it;
  delete address_mas;
  delete socket;
  delete timer;
  delete result;
}

QString My_Geocode::Geocode(){
  return "Geocode";
}

void My_Geocode::setGeocode(QString){}

/*void My_Geocode::geocoding_test()
{
  emit getcode("OpenStreetMap", "", 55.7504, 37.6175);
  emit getcode("MapBox", "",55.7558, 37.6178);
  emit getcode("YandexMap",  "", 55.7532, 37.6225);
}*/

QVariantList My_Geocode::load_in_file(QString path)
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
}

void My_Geocode::set_address_list(QVariantList list)
{
  delete address_mas;
  address_mas = new QVariantList(list);
  for(auto &x : *address_mas)
     qDebug() << x.toString();
}

void My_Geocode::get_rez(const QUrl &path)
{
  QFile f(path.fileName());

  if(!f.open(QFile::WriteOnly | QIODevice::Text)) return;
  QTextStream writeStream(&f);
  /*for(auto &it : service_list){
      auto rez = it->get_rez_data();
      for(auto &x : *rez){
          writeStream << x.address << " " << x.lat << " " << x.lon << "\n";
        }
      writeStream  << "\n";
    }*/
  f.close();
}

QVariantList My_Geocode::get_all_service_name()
{
  /*QVariantList list;
  for(auto &it : service_list){
      list.append(it->get_service_name());
    }*/
  return service_list;
}

QVariantMap My_Geocode::get_all_API_key()
{
  /*QVariantList list;
  for(auto &it : service_list){
      list.append(it->get_API_key());
    }*/
  return service_API_key;
}

void My_Geocode::load_API_key(const QVariantMap& key_map)
{
  QJsonObject obj;
  obj.insert("service_key",QJsonObject::fromVariantMap(key_map));
  QJsonDocument doc;
  /*for(auto &it : service_list){
      //qDebug() <<  key_map.value(it->get_service_name()).toString();
      obj.insert(it.toString(),key_map.value(it.toString()).toString());
    }*/
  obj.insert("type",set_service_key);
  doc.setObject(obj);
  socket->write(doc.toJson());
  service_API_key = key_map;
  /*QFile f("./key.json");

  if(!f.open(QFile::ReadOnly | QIODevice::Text)) return;
  //QTextStream readStream(&f);
  QJsonParseError docErr;
  QJsonDocument doc = QJsonDocument::fromJson(f.readAll(),&docErr);
  if (docErr.error != QJsonParseError::NoError){
      qDebug() << "key.json: json parse error";
      return;
    }
  for(auto &it : service_list){
      service_API_key[it.toString()] = doc.object().value(it.toString());
      //it->set_API_key(doc.object().value(it->get_service_name()).toString());
      //qDebug() << doc.object().value(it->get_service_name()).toString();
    }
  f.close();*/
}

void My_Geocode::download_API_key()
{
  socket->write("{\"type\":" + QByteArray::number(get_service_key) + "}");
  /*QFile jsonFile("./key.json");
  jsonFile.open(QFile::WriteOnly);
  QJsonObject obj;
  QJsonDocument doc;
  for(auto &it : service_list){
      //qDebug() <<  key_map.value(it->get_service_name()).toString();
      obj.insert(it.toString(),key_map.value(it.toString()).toString());
    }
  doc.setObject(obj);
  jsonFile.write(doc.toJson());
  //qDebug() << obj;
  jsonFile.close();*/
}

/*void My_Geocode::onfinish_geocoding_list(Service* s)
{
  qDebug() << s->get_service_name() << " " << s->get_time() << "ms";
  auto rez = s->get_rez_data();
  for(auto &x : *rez){
      //if(x.address != "No result")
        emit getcode(s->get_service_name(), x.address, x.lat, x.lon);
    }
}*/

/*void My_Geocode::geocoding(QString q)
{
  //for(auto &it : service_list) it->geocoding_list(address_mas);
}*/

void My_Geocode::direct_geocoding_list(const QVariantMap &check_map)
{  
  /*for(auto &it : service_list) {
      if(check_map.value(it->get_service_name()).toBool() == true)
        it->geocoding_list(address_mas);
    }*/
  QJsonObject obj_rez;
  obj_rez.insert("type",geocoding_address_list);
  obj_rez.insert("geocoding_type", "direct");
  obj_rez.insert("service_check",QJsonObject::fromVariantMap(check_map));
  obj_rez.insert("address_list",QJsonArray::fromVariantList(*address_mas));
  socket->write(QJsonDocument(obj_rez).toJson());
  qDebug() << obj_rez;
}

void My_Geocode::reverse_geocoding_list(const QVariantMap &check_map)
{
  QJsonArray arr;
  for(auto &x : *address_mas){
      QRegExp rx(".*(\\d+\\.\\d+).*(\\d+\\.\\d+).*");
      rx.indexIn(x.toString());
      QJsonObject obj;
      if(!rx.cap(1).isEmpty() && !rx.cap(2).isEmpty()){
        obj.insert("lat",rx.cap(1).toDouble());
        obj.insert("lon",rx.cap(2).toDouble());
        arr.push_back(obj);
        }
      //qDebug() << rx.cap(1).toDouble() << " " << rx.cap(2).toDouble();
    }
  QJsonObject obj_rez;
  obj_rez.insert("type",geocoding_address_list);
  obj_rez.insert("geocoding_type", "reverse");
  obj_rez.insert("service_check",QJsonObject::fromVariantMap(check_map));
  obj_rez.insert("address_list",arr);
  socket->write(QJsonDocument(obj_rez).toJson());
}

void My_Geocode::onReadyRead(){
  bool completed = true;

  QByteArray data = socket->readAll();
  while(completed == false) //проверка полноты полученного сообщения
    {
      if(message_size != data.size())
        {
          data += socket->readAll();
          socket->waitForReadyRead(500);
        }
      else
        {
          completed = true;
          break;
        }
    }
  QJsonParseError docError;
  QJsonDocument doc = QJsonDocument::fromJson(data,&docError);  //перевод в JSON
  qDebug() << doc;
  if(docError.error == QJsonParseError::NoError){
      switch (doc.object().value("type").toInt()) //получения типа сообщения
        {
        default:
          qDebug() << "Unknown data type.";
        case geocoding_address_list:
          {
            for(auto &it : service_list)
              {
                QVariantList arr = doc.object().value(it.toString()).toArray().toVariantList();
                //arr.toVariantList();
                (*result)[it.toString()] = arr;
                emit getcode(it.toString(),arr);
                /*for(int i = 0; i < arr.size(); i++)
                  {
                    QJsonObject x = arr[i].toObject();
                    qDebug() << it.toString() << x.value("address").toString() << x.value("lat").toDouble() << x.value("lon").toDouble();
                    //emit getcode(it.toString(), x.value("address").toString(), x.value("lat").toDouble(), x.value("lon").toDouble());
                  }*/
              }
            break;
          }
        case size:
          {
            message_size = doc.object().value("size").toInt();  //размер сообщения
            completed = false;
            break;
          }
        case get_service_key:
          {
            QJsonArray arr = doc.object().value("service_list").toArray();
            service_list.clear();
            service_API_key.clear();
            service_list = arr.toVariantList();
            service_API_key = doc.object().value("service_API_key").toObject().toVariantMap();
            /*for(int i = 0; i < arr.size(); i++){
                service_list.push_back(arr[i].toString());
                service_API_key.insert(arr[i].toString(),doc.object().value("service_API_key").toObject().value(arr[i].toString()).toString());
                //qDebug() << arr[i].toString() << "   " <<doc.object().value("service_API_key").toObject().value(arr[i].toString()).toString();
              }*/
            emit finish_download_key();
          }
        }
    }
  else
    qDebug() << "Formt error." + docError.errorString();
}

void My_Geocode::connection()
{
  socket->connectToHost("127.0.0.1",27100);
  if(socket->waitForConnected(50))  //ожедание подключения
    {
      qDebug("Connect server.");
      timer->stop();
      download_API_key();
    }
  //else
    //qDebug("Not connect server.");
}

void My_Geocode::onDisconnected()
{
  socket->close();
  //socket->deleteLater();
  timer->start();
  qDebug("Not connect server.");
}
