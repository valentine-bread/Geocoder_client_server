#ifndef INCLUDING_H
#define INCLUDING_H

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonParseError>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDebug>
//#include <QtPositioning/QGeoAddress>
#include <QUrlQuery>
#include <QFile>
#include <QTime>
#include <QTcpSocket>
#include <QTimer>

struct coordinate{
  QString addres;
  double lat;
  double lon;
};

#endif // INCLUDING_H
