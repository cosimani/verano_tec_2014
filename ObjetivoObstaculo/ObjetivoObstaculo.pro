######################################################################
# Automatically generated by qmake (2.01a) jue 15. dic 16:11:29 2011
######################################################################

QT += opengl network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets webkitwidgets serialport

TEMPLATE = app
TARGET = TU-RA
DEPENDPATH += .
INCLUDEPATH += .

INCLUDEPATH += "/opt/tuproject"


INCLUDEPATH += "/opt/ros/hydro/include"
INCLUDEPATH += "/opt/ros/hydro/include/libfreenect"

LIBS += "/home/osi/Qt5.3.1/5.3/gcc/plugins/sqldrivers/libqsqlmysql.so"

LIBS += "/opt/ros/hydro/lib/libfreenect.so"

INCLUDEPATH += "/usr/include/libusb-1.0"
LIBS += "/usr/lib/i386-linux-gnu/libusb-1.0.so"

LIBS += "/opt/ros/hydro/lib/libroscpp.so"
LIBS += "/opt/ros/hydro/lib/libxmlrpcpp.so"
LIBS += "/opt/ros/hydro/lib/librosconsole.so"
LIBS += "/opt/ros/hydro/lib/librostime.so"
LIBS += "/opt/ros/hydro/lib/librosconsole_log4cxx.so"
LIBS += "/opt/ros/hydro/lib/librosconsole_backend_interface.so"
LIBS += "/opt/ros/hydro/lib/libroscpp_serialization.so"
LIBS += "/opt/ros/hydro/lib/libcpp_common.so"
LIBS += "/opt/ros/hydro/lib/libconsole_bridge.so"
LIBS += "/opt/ros/hydro/lib/libimage_transport.so"
LIBS += "/opt/ros/hydro/lib/libactionlib.so"

LIBS += "/usr/lib/libboost_thread.so"

LIBS += "/opt/ros/hydro/lib/libopencv_core.so"
LIBS += "/opt/ros/hydro/lib/libopencv_highgui.so"
LIBS += "/opt/ros/hydro/lib/libopencv_imgproc.so"
LIBS += "/opt/ros/hydro/lib/libopencv_objdetect.so"
LIBS += "/opt/ros/hydro/lib/libopencv_photo.so"

INCLUDEPATH += "/usr/local/include/aruco"
LIBS += "/usr/local/lib/libaruco.so"

INCLUDEPATH += "/usr/include/GL/"
LIBS += "/usr/lib/i386-linux-gnu/libglut.so"

INCLUDEPATH += "/usr/include/lib3ds/"
LIBS += "/usr/lib/lib3ds-1.so"

INCLUDEPATH += "/usr/include/zbar"
LIBS += "/usr/lib/libzbar.so"

SOURCES += main.cpp \
    hiloCaptura.cpp \
    escena.cpp \
    ventana.cpp \
    util.cpp \
    model3ds.cpp \
    threedwriter.cpp \
    smtp.cpp \
    configuracion.cpp \
    QKinect.cpp \
    navegador.cpp \
    facedetector.cpp \
    adminDB.cpp \
    servidor_ros.cpp \
    controladorDesplazamiento.cpp \
    ventanaconfiguracion.cpp \
    distanciador.cpp

HEADERS += \
    hiloCaptura.h \
    escena.h \
    ventana.h \
    util.h \
    model3ds.h \
    texturaComun.h \
    threedwriter.h \
    cara.h \
    smtp.h \
    configuracion.h \
    QKinect.h \
    navegador.h \
    facedetector.h \
    persona.h \
    adminDB.h \
    servidor_ros.h \
    controladorDesplazamiento.h \
    ventanaconfiguracion.h \
    distanciador.h

FORMS += \
    ventana.ui \
    navegador.ui \
    ventanaconfiguracion.ui

RESOURCES +=
