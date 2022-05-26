#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "mycore.h"
#include <QQmlContext>
#include <QQuickStyle>
#include <QIcon>

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
  QGuiApplication app(argc, argv);
  QQmlApplicationEngine engine;
  QQuickStyle::setStyle("Universal");
  QQmlContext *context = engine.rootContext();
  const QUrl url(QStringLiteral("qrc:/main.qml"));
  QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                   &app, [url](QObject *obj, const QUrl &objUrl) {
    if (!obj && url == objUrl)
      QCoreApplication::exit(-1);
  }, Qt::QueuedConnection);
  app.setWindowIcon(QIcon("://icon_win.png"));
  MyCore code;
  context->setContextProperty("Geocode", &code);
  engine.load(url);
  return app.exec();
}
