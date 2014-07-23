QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app
TARGET = Enfrentador-Server
DEPENDPATH += .
INCLUDEPATH += .

INCLUDEPATH += "/opt/tuproject"


INCLUDEPATH += "/opt/ros/hydro/include"
INCLUDEPATH += "/opt/ros/hydro/include/libfreenect"

LIBS += "/opt/ros/hydro/lib/libroscpp.so"
LIBS += "/opt/ros/hydro/lib/librosconsole.so"
LIBS += "/opt/ros/hydro/lib/libroscpp_serialization.so"


LIBS += "/opt/ros/hydro/lib/libfreenect.so"

LIBS += "/opt/ros/hydro/lib/libopencv_core.so"
LIBS += "/opt/ros/hydro/lib/libopencv_highgui.so"
LIBS += "/opt/ros/hydro/lib/libopencv_imgproc.so"
LIBS += "/opt/ros/hydro/lib/libopencv_objdetect.so"

SOURCES += main.cpp \
    hiloCaptura.cpp \
    escena.cpp \
    ventana.cpp \
    configuracion.cpp \
    QKinect.cpp \
    controladorDesplazamiento.cpp \
    enfrentador.cpp \
    servidor_ros.cpp

HEADERS += \
    hiloCaptura.h \
    escena.h \
    ventana.h \
    configuracion.h \
    QKinect.h \
    controladorDesplazamiento.h \
    enfrentador.h \
    servidor_ros.h

FORMS += \
    ventana.ui

RESOURCES +=
