#include <QApplication>

#include "ventana.h"
#include "configuracion.h"

#include "ros/ros.h"

#include <QDebug>
#include <QDesktopWidget>

int main( int argc, char** argv )  {
    QApplication a(argc, argv);

    ros::init(argc, argv, "TUBot");

    Configuracion::obtenerConfiguracion();

    QDesktopWidget * desktopWidget = QApplication::desktop();
    qDebug() << desktopWidget->screenCount();

    Ventana ventana;
    ventana.showMaximized();

    int quit = a.exec();
    ros::shutdown();

    return quit;
}
