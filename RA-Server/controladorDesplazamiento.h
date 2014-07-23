#ifndef TURTLEBOTTELEOP_H
#define TURTLEBOTTELEOP_H

#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <signal.h>
#include <termios.h>
#include <stdio.h>
#include "boost/thread/mutex.hpp"
#include "boost/thread/thread.hpp"

class ControladorDesplazamiento  {

public:
    ControladorDesplazamiento();
    ~ControladorDesplazamiento();

    void publish(double, double);  // cmd_vel_teleop

private:
    ros::NodeHandle nodoDesplazamiento;
    double lineal, angular;
    double escalaLineal, escalaAngular;
    ros::Publisher publisherDesplazamiento;
};



#endif // TURTLEBOTTELEOP_H

