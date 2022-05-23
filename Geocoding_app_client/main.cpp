#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "mycore.h"
#include <QQmlContext>
#include <QQuickStyle>

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

  MyCore code;
  //code.load_API_key();
  context->setContextProperty("Geocode", &code);

  engine.load(url);

  return app.exec();
}
