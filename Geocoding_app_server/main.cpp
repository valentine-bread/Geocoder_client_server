#include <QCoreApplication> 
#include "mytcpserver.h"

int main(int argc, char *argv[])
{
  QCoreApplication a(argc, argv);
  MyTcpServer core;
  return a.exec();
}

