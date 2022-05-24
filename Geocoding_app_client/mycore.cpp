#include "mycore.h"

MyCore::MyCore(QObject *parent)
  : QObject{parent}
{
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

MyCore::~MyCore()
{
  //for(auto &it : service_list)
    //delete it;
  delete address_mas;
  delete socket;
  delete timer;
  delete result;
}

QString MyCore::Geocode(){
  return "Geocode";
}

void MyCore::setGeocode(QString){}

QVariantList MyCore::load_out_file(QString path)
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

void MyCore::set_address_list(QVariantList list)
{
  delete address_mas;
  address_mas = new QVariantList(list);
  for(auto &x : *address_mas)
     qDebug() << x.toString();
}

void MyCore::save_in_file(const QUrl &path)
{
  QFile f(path.fileName());

  if(!f.open(QFile::WriteOnly | QIODevice::Text)) return;
  QTextStream writeStream(&f);
  for(auto &it : service_list){
      if(result->value(it.toString()).isEmpty()) continue;
      writeStream  << it.toString() << "\n";
      auto rez = result->value(it.toString());
      for(auto &x : rez){
          QJsonObject obj = x.toJsonObject();
          writeStream << obj.value("address").toString() << " " << obj.value("lat").toString() << " " << obj.value("lon").toString() << "\n";
        }
      writeStream  << "\n";
    }
  f.close();
}

const QVariantList MyCore::get_all_service_name()
{
  return service_list;
}

const QVariantMap MyCore::get_all_API_key()
{
  return service_API_key;
}

void MyCore::load_API_key(const QVariantMap& key_map)
{
  QJsonObject obj;
  obj.insert("service_key",QJsonObject::fromVariantMap(key_map));
  QJsonDocument doc;
  obj.insert("type",set_service_key);
  doc.setObject(obj);
  socket->write(doc.toJson());
  service_API_key = key_map;
}

void MyCore::load_key_out_file(const QUrl &path)
{
  QFile f(path.fileName());

  if(!f.open(QFile::ReadOnly | QIODevice::Text)) return;
  QJsonParseError docErr;
  QJsonDocument doc = QJsonDocument::fromJson(f.readAll(),&docErr);
  if (docErr.error != QJsonParseError::NoError){
      qDebug() << "key.json: json parse error";
      return;
    }
  service_API_key = doc.object().toVariantMap();
  load_API_key(service_API_key);
  emit finish_download_key();
  f.close();
}

void MyCore::download_API_key()
{
  socket->write("{\"type\":" + QByteArray::number(get_service_key) + "}");
}

void MyCore::direct_geocoding_list(const QVariantMap &check_map)
{  
  QJsonObject obj_rez;
  obj_rez.insert("type",geocoding_address_list);
  obj_rez.insert("geocoding_type", "direct");
  obj_rez.insert("service_check",QJsonObject::fromVariantMap(check_map));
  obj_rez.insert("address_list",QJsonArray::fromVariantList(*address_mas));
  socket->write(QJsonDocument(obj_rez).toJson());
  qDebug() << obj_rez;
}

void MyCore::reverse_geocoding_list(const QVariantMap &check_map)
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

void MyCore::onReadyRead(){
  data = socket->readAll();
  while(completed == false) //проверка полноты полученного сообщения
    {
      if(message_size != data.size()){
          data += socket->readAll();
          socket->waitForReadyRead(50);
        }
      else{
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
        default:{
          qDebug() << "Unknown data type.";
          break;
          }
        case geocoding_address_list:
          {
            for(auto &it : service_list)
              {
                QVariantList arr = doc.object().value(it.toString()).toArray().toVariantList();
                (*result)[it.toString()] = arr;
                emit getcode(it.toString(),arr);
              }
            emit finish_all_geocoding();
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
            emit finish_download_key();
            break;
          }
        case set_service_key:
          {
            QJsonObject obj_rez;
            obj_rez.insert("type",get_service_key);
            obj_rez.insert("service_list", QJsonArray::fromVariantList(get_all_service_name()));
            obj_rez.insert("service_API_key", QJsonObject::fromVariantMap(get_all_API_key()));
            socket->write(QJsonDocument(obj_rez).toJson());
            socket->waitForBytesWritten(500);
            break;
          }
        }
    }
  else{
    qDebug() << "Formt error." + docError.errorString();
    emit finish_all_geocoding();
    }
}

void MyCore::connection()
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

void MyCore::onDisconnected()
{
  socket->close();
  //socket->deleteLater();
  timer->start();
  qDebug("Not connect server.");
}
