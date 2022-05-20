TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
QT += location core network

SOURCES += \
        main.cpp \
        my_geocode.cpp \
        mytcpserver.cpp \
        service.cpp \
        service/service_mapbox.cpp \
        service/service_osm.cpp \
        service/service_ya.cpp

HEADERS += \
    including.h \
    my_geocode.h \
    mytcpserver.h \
    service.h \
    service/service_mapbox.h \
    service/service_osm.h \
    service/service_ya.h
