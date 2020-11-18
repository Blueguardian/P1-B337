#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>

double coordxCallback(const std_msgs::Int8::ConstPtr& msgx)
{
   ROS_INFO(("x-coord received: ", msgx->data))
   double xcoord = msg->data;
   return xcoord;
}

double coordxCallback(const std_msgs::Int8::ConstPtr& msgy)
{
   ROS_INFO(("y-coord received: ", msgy->data))
   double ycoord = msg->data;
   return ycoord;
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "listener_test");
    ros::NodeHandle n;

    ros::Subscriber user_input_x = n.subscribe("user_inputx", 1, coordxCallback);
    ros::Subscriber user_input_y = n.subscribe("user_inputy", 1, xoordyCallback);

    ros::spin();

    return 0;
}