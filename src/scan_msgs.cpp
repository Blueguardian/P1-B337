#include <ros/ros.h>
#include <sensor_msgs/LaserScan.h>
/*Here, we include the sensor_msgs/LaserScan message that we want to send over the wire.*/

int main(int argc, char** argv){
  ros::init(argc, argv, "laser_scan_publisher");


  ros::NodeHandle n;
  ros::Publisher scan_pub = n.advertise<sensor_msgs::LaserScan>("scan", 50);
/*This code creates a ros::Publisher that is later used to send LaserScan messages using ROS.*/


  unsigned int num_readings = 100;
  double laser_frequency = 40;
  double ranges[num_readings];
  double intensities[num_readings];
  /*Here we're just setting up storage for the dummy data that we're going to use to populate our scan. A real application would pull this data from their laser driver.*/

  int count = 0;
  ros::Rate r(1.0);
  while(n.ok()){
    //generate some fake data for our laser scan
    for(unsigned int i = 0; i < num_readings; ++i){
      ranges[i] = count;
      intensities[i] = 100 + count;
    }
    ros::Time scan_time = ros::Time::now();
  /*Populate the dummy laser data with values that increase by one every second.*/

    //populate the LaserScan message
    sensor_msgs::LaserScan scan;
    scan.header.stamp = scan_time;
    scan.header.frame_id = "laser_frame";
    scan.angle_min = -1.57;
    scan.angle_max = 1.57;
    scan.angle_increment = 3.14 / num_readings;
    scan.time_increment = (1 / laser_frequency) / (num_readings);
    scan.range_min = 0.0;
    scan.range_max = 100.0;

    scan.ranges.resize(num_readings);
    scan.intensities.resize(num_readings);
    for(unsigned int i = 0; i < num_readings; ++i){
      scan.ranges[i] = ranges[i];
      scan.intensities[i] = intensities[i];
    }
/*Create a scan_msgs::LaserScan message and fill it with the data that we've generated in preparation to send it over the wire.*/

    scan_pub.publish(scan);
    /*Publish the message over ROS.*/
    ++count;
    r.sleep();
  }
}