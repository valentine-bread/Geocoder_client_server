#include "my_geocode.h"

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
  addres_mas = new QVariantList();
  qDebug() << "Ok";  
}

My_Geocode::~My_Geocode()
{
  //for(auto &it : service_list)
    //delete it;
  delete addres_mas;
  delete socket;
  delete timer;
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
  if (path.isEmpty()) return *addres_mas;
  path.remove(0,8);
  QFile f(path);
  if(!f.open(QFile::ReadOnly | QIODevice::Text)) return *addres_mas;
  addres_mas->clear();
  while(!f.atEnd()){
      auto line = f.readLine();
      addres_mas->push_back(line);
    }
  f.close();
  qDebug() << "rez";
  for(auto &x : *addres_mas){
      qDebug() << x.toString();
    }
  f.close();
  return *addres_mas;
}

void My_Geocode::set_addres_list(QVariantList list)
{
  delete addres_mas;
  addres_mas = new QVariantList(list);
  for(auto &x : *addres_mas)
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
          writeStream << x.addres << " " << x.lat << " " << x.lon << "\n";
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
      service_API_key[it.toString()] = doc.object().value(it.toString());
      //it->set_API_key(doc.object().value(it->get_service_name()).toString());
      //qDebug() << doc.object().value(it->get_service_name()).toString();
    }
  f.close();
}

void My_Geocode::download_API_key()
{
  socket->write("{\"type\":" + QByteArray::number(service_key) + "}");
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
      //if(x.addres != "No result")
        emit getcode(s->get_service_name(), x.addres, x.lat, x.lon);
    }
}*/

/*void My_Geocode::geocoding(QString q)
{
  //for(auto &it : service_list) it->geocoding_list(addres_mas);
}*/

void My_Geocode::geocoding_list(const QVariantMap &check_map)
{  
  /*for(auto &it : service_list) {
      if(check_map.value(it->get_service_name()).toBool() == true)
        it->geocoding_list(addres_mas);
    }*/
  QJsonObject obj_rez;
  obj_rez.insert("type",geocoding_addres_list);
  obj_rez.insert("service_check",QJsonObject::fromVariantMap(check_map));
  obj_rez.insert("addres_list",QJsonArray::fromVariantList(*addres_mas));
  socket->write(QJsonDocument(obj_rez).toJson());
  qDebug() << obj_rez;
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
        case geocoding_addres_list:
          {
            for(auto &it : service_list)
              {
                QJsonArray arr = doc.object().value(it.toString()).toArray();
                for(int i = 0; i < arr.size(); i++)
                  {
                    QJsonObject x = arr[i].toObject();
                    qDebug() << it.toString() << x.value("addres").toString() << x.value("lat").toDouble() << x.value("lon").toDouble();
                    emit getcode(it.toString(), x.value("addres").toString(), x.value("lat").toDouble(), x.value("lon").toDouble());
                  }
              }
            break;
          }
        case size:
          {
            message_size = doc.object().value("size").toInt();  //размер сообщения
            completed = false;
            break;
          }
        case service_key:
          {
            QJsonArray arr = doc.object().value("service_list").toArray();
            for(int i = 0; i < arr.size(); i++){
                service_list.push_back(arr[i].toString());
                service_API_key.insert(arr[i].toString(),doc.object().value("service_API_key").toObject().value(arr[i].toString()).toString());
                qDebug() << arr[i].toString() << "   " <<doc.object().value("service_API_key").toObject().value(arr[i].toString()).toString();
              }
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
  else
    qDebug("Not connect server.");
}

void My_Geocode::onDisconnected()
{
  socket->close();
  //socket->deleteLater();
  timer->start();
  qDebug("Not connect server.");
}
