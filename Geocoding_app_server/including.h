#ifndef INCLUDING_H
#define INCLUDING_H

#include <QCoreApplication>
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
#include <QTcpServer>
#include <QTcpSocket>
#include <QGeoAddress>
#include <QGeoLocation>
#include <QGeoCoordinate>
#include <QNetworkDiskCache>
#include <QDir>

struct coordinate{
  QString address;
  double lat;
  double lon;
  QGeoLocation location;
};

#endif // INCLUDING_H
