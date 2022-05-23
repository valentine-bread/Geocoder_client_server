#include "mytcpserver.h"

MyTcpServer::MyTcpServer(QObject *parent):QTcpServer(parent)
{
  if(listen(QHostAddress::Any,27100)) //прослушивание
    qDebug() << "Listening...";
  else
    qDebug() << "Start listening error." << errorString();

}

void MyTcpServer::incomingConnection(qintptr socketDescriptor)
{
  geocoder_mas.push_back(new My_Geocode());//добавление нового кодера
  QTcpSocket* tmp_socket = geocoder_mas.last()->socket;
  My_Geocode* geocoder = geocoder_mas.last();
  geocoder->load_API_key();
  connect(geocoder,SIGNAL(finish_geocoding_all(QJsonDocument&)),this,SLOT(onfinish_geocoding_all(QJsonDocument&)),Qt::DirectConnection);
  tmp_socket->setSocketDescriptor(socketDescriptor);
  connect(tmp_socket,SIGNAL(readyRead()),this,SLOT(onReadyRead()),Qt::DirectConnection);  //DirectConnection - явное подключение слотов в новый поток
  connect(tmp_socket,SIGNAL(disconnected()),this,SLOT(onDisconnected()),Qt::DirectConnection);  //Сигналы от сокета
  qDebug() << "Connected client: " << socketDescriptor;
}

void MyTcpServer::onReadyRead()
{
  QTcpSocket* tmp_socket = qobject_cast<QTcpSocket*>(sender()); //получение указателя на сокет
  My_Geocode* geocoder = qobject_cast<My_Geocode*>(tmp_socket->parent()); //получение на геокодер
  QJsonParseError docError;
  QJsonDocument doc = QJsonDocument::fromJson(tmp_socket->readAll(),&docError);
  qDebug() << doc;
  if(docError.errorString() == "no error occurred")
    {
      //QByteArray itog;
      switch (doc.object().value("type").toInt())   //определения типа сообщения
        {
        default:
          qDebug() << "Unknown data type.";
        case geocoding_address_list:
          {
            geocoder->geocoding_list(doc.object().value("service_check").toObject().toVariantMap(),
                                     doc.object().value("address_list").toArray().toVariantList(),
                                     doc.object().value("geocoding_type").toString());
            tmp_socket->waitForBytesWritten(500);
            break;
          }
        case get_service_key:
          {
            QJsonObject obj_rez;
            obj_rez.insert("type",get_service_key);
            obj_rez.insert("service_list", QJsonArray::fromVariantList(geocoder->get_all_service_name()));
            obj_rez.insert("service_API_key", QJsonObject::fromVariantMap(geocoder->get_all_API_key()));
            tmp_socket->write(QJsonDocument(obj_rez).toJson());
            tmp_socket->waitForBytesWritten(500);
            break;
          }
        case set_service_key:
          {
            geocoder->download_API_key(doc.object().value("service_key").toObject().toVariantMap());
            break;
          }
        }
    }
  else
    {
      qDebug() << "Formt error." + docError.errorString();
    }
}

void MyTcpServer::onDisconnected()
{
  QTcpSocket* tmp_socket = qobject_cast<QTcpSocket*>(sender()); //получение указателя на сокет
  qDebug() << "Disconnected client.";
  tmp_socket->close();
  //tmp_socket->deleteLater();
  tmp_socket->parent()->deleteLater();
}

void MyTcpServer::onfinish_geocoding_all(QJsonDocument &rez)
{
  QTcpSocket* tmp_socket =  qobject_cast<My_Geocode*>(sender())->socket;
  QJsonObject obj = rez.object();
  obj.insert("type",geocoding_address_list);
  rez.setObject(obj);
  QByteArray itog(rez.toJson());
  //qDebug() << ("{\"type\":" + QByteArray::number(size) + ",\"size\":"+ QByteArray::number(itog.size()) + "}");
  tmp_socket->write("{\"type\":" + QByteArray::number(size) + ",\"size\":"+ QByteArray::number(itog.size()) + "}"); //отправка размера сообщения
  tmp_socket->waitForBytesWritten(500);
  tmp_socket->write(itog);  //отправка тела сообщения
  qDebug() << "Write message client " << tmp_socket->socketDescriptor();
  itog.clear();
}
