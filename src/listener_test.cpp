#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>

double xcoord;
double ycoord;

void coordCallback(const x_300_master::coord::ConstPtr& msg)
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

   ROS_INFO(("x-coordinate stored: ", xcoord));
   ROS_INFO(("y-coordinate stored: ", ycoord));
   ros::spinOnce();

    return 0;
}