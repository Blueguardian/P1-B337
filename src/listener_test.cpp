#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>

struct coordset
{
   double x;
   double y;
};

double coordCallback(const ::x_300_master::coord::ConstPtr& msg)
{
   coordset coordset;
   ROS_INFO(("x-coord received: ", msg->coordx))
   ROS_INFO(("y-coord received: ", msg->coordy))
   double coordset.x = msg->x;
   double coordset.y = msg->y;
   return coordset;
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "listener_test");
    ros::NodeHandle n;

    ros::Subscriber user_input = n.subscribe("user_input", 1, coordCallback);


    ros::spin();

    return 0;
}