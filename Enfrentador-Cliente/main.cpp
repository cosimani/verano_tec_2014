#include "principal.h"
#include <QApplication>

#include "ros/ros.h"

int main(int argc, char** argv)
{
    QApplication a(argc, argv);

    ros::init(argc, argv, "Enfrentador_Cliente");

    Principal w;
    w.show();
    
    int quit = a.exec();
    ros::shutdown();

    return quit;
}
