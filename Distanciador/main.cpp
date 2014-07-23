#include <QApplication>

#include "ventana.h"
#include "configuracion.h"

#include "ros/ros.h"

#include <QDebug>
#include <QDesktopWidget>

int main( int argc, char** argv )  {
    QApplication a(argc, argv);

    Configuracion::obtenerConfiguracion();

    Ventana ventana;
    ventana.show();

    int quit = a.exec();

    return quit;
}
