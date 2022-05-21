#include "service.h"

Service::Service(QObject *parent)
  : QObject{parent}
{
  manager = new QNetworkAccessManager(this);
  connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(slot_manager(QNetworkReply*)));
  rez_data = new QList<coordinate>();
}

Service::~Service()
{
  delete manager;
}

void Service::geocoding_list(const QVariantList& q)
{
  timer = QTime::currentTime();
  if(q.isEmpty()) return;
  rez_data->clear();
  for(auto &x : (q)){
      rez_data->push_back(coordinate{x.toString()});
    }
  counter = rez_data->begin();
  this->geocoding();
}

void Service::geocoding()
{  
  QNetworkRequest _request(request(&counter->address));
  _request.setRawHeader("Accept-Language", "ru");
  manager->get(_request);
}

void Service::slot_manager(QNetworkReply *rez)
{
  if(rez->error() == QNetworkReply::NoError){
      QJsonParseError docErr;
      QJsonDocument doc = QJsonDocument::fromJson(rez->readAll(),&docErr);
      if (docErr.error == QJsonParseError::NoError){
          QGeoLocation code = reply(&doc);
          counter->location = code;
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
      geocoding();
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
