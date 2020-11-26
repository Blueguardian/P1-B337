#include <ros/ros.h>
#include <sensor_msgs/PointCloud.h>
/*Includes the sensor_msgs/PointCloud message.*/

int main(int argc, char** argv){
  ros::init(argc, argv, "point_cloud_publisher");

  ros::NodeHandle n;
  ros::Publisher cloud_pub = n.advertise<sensor_msgs::PointCloud>("cloud", 50);
  /*Creates the ros::Publisher that we'll use to send PointCloud messages out over the wire.*/

  unsigned int num_points = 100;

  int count = 0;
  ros::Rate r(1.0);
  while(n.ok()){
    sensor_msgs::PointCloud cloud;
    cloud.header.stamp = ros::Time::now();
    cloud.header.frame_id = "sensor_frame";
/*Fill in the header of the PointCloud message that we'll send out with the relevant frame and timestamp information.*/


    cloud.points.resize(num_points);
/*Set the number of points in the point cloud so that we can populate them with dummy data.*/
    //we'll also add an intensity channel to the cloud


    cloud.channels.resize(1);
    cloud.channels[0].name = "intensities";
    cloud.channels[0].values.resize(num_points);
/*Adds a channel called "intensity" to the PointCloud and sizes it to match the number of points that we'll have in the cloud.*/

    //generate some fake data for our point cloud
    for(unsigned int i = 0; i < num_points; ++i){
      cloud.points[i].x = 1 + count;
      cloud.points[i].y = 2 + count;
      cloud.points[i].z = 3 + count;
      cloud.channels[0].values[i] = 100 + count;
    }
/*Populates the PointCloud message with some dummy data. Also, populates the intensity channel with dummy data.*/



    cloud_pub.publish(cloud);
    ++count;
    r.sleep();
    /*Publishes the PointCloud using ROS.*/
  }
}