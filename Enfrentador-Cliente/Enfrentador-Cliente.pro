QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Enfrentador-Cliente
TEMPLATE = app

INCLUDEPATH += "/opt/ros/hydro/include"
INCLUDEPATH += "/opt/ros/hydro/include/libfreenect"
INCLUDEPATH += "/opt/tuproject"

LIBS += "/opt/ros/hydro/lib/libroscpp.so"
LIBS += "/opt/ros/hydro/lib/librosconsole.so"
LIBS += "/opt/ros/hydro/lib/libroscpp_serialization.so"

LIBS += "/opt/ros/hydro/lib/libopencv_core.so"
LIBS += "/opt/ros/hydro/lib/libopencv_highgui.so"

LIBS += "/opt/ros/hydro/lib/libfreenect.so"

SOURCES += main.cpp\
        principal.cpp \
    controladorDesplazamiento.cpp

HEADERS  += principal.h \
    controladorDesplazamiento.h \
    cliente_ros.h

FORMS    += principal.ui
