#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/Image.h>
#include <opencv2/core.hpp>
#include <std_msgs/Bool.h>

bool take_picture_check;
ros::NodeHandle* nhptr;

void imageCallback(const sensor_msgs::ImageConstPtr& msg);
void takepic_cb(const std_msgs::Bool::ConstPtr& pic);

int main(int argc, char **argv)
{
  ros::init(argc, argv, "exhibit_recorder");
  ros::NodeHandle nh;
  nhptr = &nh;
  image_transport::ImageTransport it(nh); //Conversion function

  image_transport::Subscriber sub = it.subscribe("/camera/rgb/image_raw", 1, imageCallback); //Subscribe to the camera
  ros::Subscriber take_picture = nh.subscribe<std_msgs::Bool>("take_picture", 1, takepic_cb); //Subscribe to the main function, to know when to take a picture
  while(ros::ok())
  if(take_picture_check == true)
  {
  ros::Duration(10); //Wait this long before closing again
  take_picture_check = false; //Reset
  }
  ros::spin();
}

void imageCallback(const sensor_msgs::ImageConstPtr& msg) //Callback function for camera input
{
  //This function takes in the picture info, and tries printing it on the screen, if the
  //image cannot be converted to type bgr8, then it will throw an error instead.

  //Beginning of function

  try
  {
    ROS_INFO("Trying to show picture..");
    ros::Publisher image_pub = nhptr->advertise<sensor_msgs::Image>("image_view", 1);
    cv::waitKey(30); //Wait
  }
  catch (cv_bridge::Exception& e) //If the picture cannot be converted to bgr8 and shown on the screen
  {
    ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str()); //Tell the user that the picture cannot be shown on the screen
  }
}

void takepic_cb(const std_msgs::Bool::ConstPtr& pic) //Callback function for taking a picture
{
  if(pic == false){
  ROS_INFO("Taking picture.."); //Tell the user that the image is being processed on the screen
  take_picture_check = true; //Tell it to take the picture
  }
}