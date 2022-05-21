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
#include <QTcpServer>
#include <QTcpSocket>
#include <QGeoAddress>
#include <QGeoLocation>
#include <QGeoCoordinate>

struct coordinate{
  QString address;
  QGeoLocation location;
};

#endif // INCLUDING_H
