#include <QApplication>

#include "ventana.h"
#include "configuracion.h"
#include "ros/ros.h"

int main( int argc, char** argv )  {
    QApplication a(argc, argv);

    Configuracion::obtenerConfiguracion();

    ros::init(argc, argv, "Enfrentador_Server");

    Ventana ventana;
    ventana.show();

    return a.exec();
}
