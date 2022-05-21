#include <QCoreApplication>
#include <QTextStream>
#include <QTextCodec>
#include "mytcpserver.h"

int main(int argc, char *argv[])
{
  QCoreApplication a(argc, argv);
  QTextStream outStream(stdout);
  outStream.setCodec(QTextCodec::codecForName("cp866"));
  MyTcpServer core;
  return a.exec();
}

