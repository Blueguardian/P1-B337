#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <std_msgs/Int32.h>

double xcoord;
double ycoord;

//No explanation needed here, since this is only for testing and may be omitted when testing turns over to master.cpp

void coordCallback1(const std_msgs::Int32::ConstPtr& msg)
{
   xcoord = (double)msg->data;
   xcoord = (xcoord/1000)
   std::cout<<"x-coord received:"<<xcoord<<std::endl;

}

void coordCallback2(const std_msgs::Int32::ConstPtr& msg)
{
   ycoord = (double)msg->data;
   ycoord = (ycoord/1000)
   std::cout<<"y-coord received:"<<ycoord<<std::endl;
   
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "listener_test");
    ros::NodeHandle n;

    ros::Subscriber user_input123 = n.subscribe("user_input1", 1, coordCallback1);
    ros::Subscriber user_input321 = n.subscribe("user_input2", 1, coordCallback2);

   std::cout<<"x-coordinate stored:"<<xcoord<<std::endl;
   std::cout<<"y-coordinate stored:"<<ycoord<<std::endl;

   ros::spinOnce();

    return 0;
}