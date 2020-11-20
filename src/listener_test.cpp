#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <x_300_master/coord.h>
#include <std_msgs/Float32.h>

double xcoord;
double ycoord;

//No explanation needed here, since this is only for testing and may be omitted when testing turns over to master.cpp

void coordCallback(const (x_300_master)::coord::ConstPtr& msg)
{
   ROS_INFO(("x-coord received: %f", msg->coordx));
   ROS_INFO(("y-coord received: %f", msg->coordy));
   xcoord = msg.coordx;
   ycoord = msg.coordy;
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "listener_test");
    ros::NodeHandle n;

    ros::Subscriber user_input = n.subscribe("user_input", 1, coordCallback);

   ROS_INFO(("x-coordinate stored: %f", xcoord));
   ROS_INFO(("y-coordinate stored: %f", ycoord));
   ros::spinOnce();

    return 0;
}