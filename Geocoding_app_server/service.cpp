#include "service.h"

Service::Service(QObject *parent)
  : QObject{parent}
{
  manager = new QNetworkAccessManager(this);
  diskCache = new QNetworkDiskCache(this);
  diskCache->setCacheDirectory( QCoreApplication::applicationDirPath() + QString("\\cache") );
  manager->setCache(diskCache);
  connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(slot_manager(QNetworkReply*)));
  rez_data = new QList<coordinate>();
  _request = new QNetworkRequest();
  _request->setRawHeader("Accept-Language", "ru");
  _request->setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::PreferCache);
}

Service::~Service()
{
  delete manager;
  delete rez_data;
  delete _request;
}

QGeoLocation Service::reverse_reply(const QJsonDocument *doc)
{
  return reply(doc);
}

void Service::geocoding_list(const QVariantList& q, const QString& type)
{
  timer = QTime::currentTime();
  if(q.isEmpty()) return;
  rez_data->clear();
  if(type == "direct"){
    type_geocoding = 0;
    for(auto &x : (q)){
        rez_data->push_back(coordinate{x.toString()});
      }
    counter = rez_data->begin();
    this->direct_geocoding();
    }
  else{
    type_geocoding = 1;

    for(auto &x : (q)){
        QJsonObject obj = x.toJsonObject();
        rez_data->push_back(coordinate{x.toString(),obj.value("lat").toDouble(),obj.value("lon").toDouble()});
      }
    counter = rez_data->begin();
    this->reverse_geocoding();
    }
}

void Service::direct_geocoding()
{  
  _request->setUrl(direct_request(counter->address));
  manager->get(*_request);
}

void Service::reverse_geocoding()
{
  _request->setUrl(reverse_request(counter->lat,counter->lon));
  manager->get(*_request);
}

void Service::slot_manager(QNetworkReply *rez)
{
  QVariant fromCache = rez->attribute(QNetworkRequest::SourceIsFromCacheAttribute);
  if(rez->error() == QNetworkReply::NoError){
      QJsonParseError docErr;
      QJsonDocument doc = QJsonDocument::fromJson(rez->readAll(),&docErr);
      //qDebug() << doc;
      if (docErr.error == QJsonParseError::NoError){
          QGeoLocation code;
          if (type_geocoding)  code = reverse_reply(&doc);
          else code = reply(&doc);
          counter->location = code;
          qDebug() << " cache?" << fromCache.toBool() ;
          //qDebug() << get_service_name() << " " <<  code.lat << " " << code.lon;
        }
      else
          qDebug() << "Doc Error. " + docErr.errorString();
    }
  else
      qDebug() << rez->errorString();

  counter++;
  if(counter == rez_data->end()){
      time_execution = timer.msecsTo(QTime::currentTime());
      emit finish_geocoding_list(this);
    }
  else
    if(type_geocoding) this->reverse_geocoding();
        else this->direct_geocoding();
}

const int Service::get_time()
{
  return time_execution;
}

QList<coordinate>* Service::get_rez_data()
{
  return rez_data;
}

void Service::set_API_key(QString key)
{
  API_key = key;
}

QString Service::get_API_key()
{
  return API_key;
}
