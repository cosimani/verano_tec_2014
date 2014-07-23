#include "controladorDesplazamiento.h"
#include <QDebug>

ControladorDesplazamiento::ControladorDesplazamiento() : lineal(0), angular(0), escalaLineal(1.0), escalaAngular(1.0)  {
    nodoDesplazamiento.param("scale_angular", escalaAngular, escalaAngular);
    nodoDesplazamiento.param("scale_linear", escalaLineal, escalaLineal);

    publisherDesplazamiento = nodoDesplazamiento.advertise<geometry_msgs::Twist>("/cmd_vel_mux/input/teleop", 1);
}

ControladorDesplazamiento::~ControladorDesplazamiento()  {
    publisherDesplazamiento.shutdown();
}

void ControladorDesplazamiento::publish(double angular, double linear)  {
    geometry_msgs::Twist msgDesplazamiento;
    msgDesplazamiento.angular.z = escalaAngular * angular;
    msgDesplazamiento.linear.x = escalaLineal * linear;

    publisherDesplazamiento.publish(msgDesplazamiento);
}

