#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <std_msgs/Float32.h>

double xcoord;
double ycoord;

//No explanation needed here, since this is only for testing and may be omitted when testing turns over to master.cpp

void coordCallback(const (P1-B337)::coord::ConstPtr& msg)
{

   ROS_INFO(("x-coord received: ", msg->coordx));
   ROS_INFO(("y-coord received: ", msg->coordy));
   xcoord = *msg->coordx;
   ycoord = *msg->coordy;

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