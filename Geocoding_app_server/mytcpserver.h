#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include "including.h"
#include "my_geocode.h"

class MyTcpServer : public QTcpServer
{
  Q_OBJECT

public:
  explicit MyTcpServer(QObject *parent = nullptr);
  enum Command{test, geocoding_addres_list, size, service_key};

private:
  void incomingConnection(qintptr handle);
  //My_Geocode *geocoder;
  QVector<My_Geocode*> geocoder_mas;
  /*QJsonDocument doc;
  QJsonParseError docError;
  QByteArray data;*/
  int counter = 0;
  //QByteArray query_to_byte_array(QSqlQuery*);

public slots:
  void onReadyRead();
  void onDisconnected();
  void onfinish_geocoding_all(QJsonDocument&);
};


#endif // MYTCPSERVER_H
