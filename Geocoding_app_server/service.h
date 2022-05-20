#ifndef SERVICE_H
#define SERVICE_H

#include "including.h"

class Service : public QObject
{
  Q_OBJECT
public:
  explicit Service(QObject *parent = nullptr);
  ~Service();
  virtual QUrl request(const QString* q) = 0;
  virtual coordinate reply(const QJsonDocument* doc) = 0;
  virtual QString get_service_name() = 0;
  const int get_time();
  void geocoding();
  void geocoding_list(const QVariantList& q);
  //void next_step(double lat = 0, double lon = 0);
  QList<coordinate>* get_rez_data();
  void set_API_key(QString key);
  QString get_API_key();


signals:
  //void getcode(QString name, double lat, double lon);
  void finish_geocoding_list(Service*);

protected slots:
   void slot_manager(QNetworkReply* rez);

protected:
  QNetworkAccessManager* manager;
  QList<coordinate>* rez_data;
  QList<coordinate>::iterator counter;
  QTime timer;
  QString API_key;
  int time_execution;
};

#endif // SERVICE_H
