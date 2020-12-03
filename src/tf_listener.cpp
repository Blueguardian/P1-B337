#include <ros/ros.h>
#include <geometry_msgs/PointStamped.h>
#include <tf/transform_listener.h>

/*Here, we include the tf/transform_listener.h header file that we'll need to create a tf::TransformListener. 
A TransformListener object automatically subscribes to the transform message topic over ROS and manages all transform data coming in over the wire.*/

void transformPoint(const tf::TransformListener& listener)
/*We'll create a function that, given a TransformListener, takes a point in the "base_laser" frame and transforms it to the "base_link" frame. 
This function will serve as a callback for the ros::Timer created in the main() of our program and will fire every second.*/
{
  //we'll create a point in the base_laser frame that we'd like to transform to the base_link frame
  geometry_msgs::PointStamped laser_point;
  laser_point.header.frame_id = "base_laser";
/*Here, we'll create our point as a geometry_msgs::PointStamped. 
The "Stamped" on the end of the message name just means that it includes a header, allowing us to associate both a timestamp and a frame_id with the message.*/

  //we'll just use the most recent transform available for our simple example
  laser_point.header.stamp = ros::Time();
/* We'll set the stamp field of the laser_point message to be ros::Time() which is a special time value that allows us to ask the TransformListener for the latest available transform.*/

  //just an arbitrary point in space
  laser_point.point.x = 1.0;
  laser_point.point.y = 0.2;
  laser_point.point.z = 0.0;
/* As for the frame_id field of the header, we'll set that to be "base_laser" because we're creating a point in the "base_laser" frame. 
 Finally, we'll set some data for the point.... picking values of x: 1.0, y: 0.2, and z: 0.0.*/


  try{
    geometry_msgs::PointStamped base_point;
    listener.transformPoint("base_link", laser_point, base_point);

    ROS_INFO("base_laser: (%.2f, %.2f. %.2f) -----> base_link: (%.2f, %.2f, %.2f) at time %.2f",
        laser_point.point.x, laser_point.point.y, laser_point.point.z,
        base_point.point.x, base_point.point.y, base_point.point.z, base_point.header.stamp.toSec());
  }
/*Now that we have the point in the "base_laser" frame we want to transform it into the "base_link" frame. 
To do this, we'll use the TransformListener object, and call transformPoint() with three arguments:
the name of the frame we want to transform the point to ("base_link" in our case), the point we're transforming, and storage for the transformed point.
So, after the call to transformPoint(), base_point holds the same information as laser_point did before only now in the "base_link" frame.*/
  catch(tf::TransformException& ex){
    ROS_ERROR("Received an exception trying to transform a point from \"base_laser\" to \"base_link\": %s", ex.what());
  }
/*If for some reason the base_laser → base_link transform is unavailable (perhaps the tf_broadcaster is not running),
the TransformListener may throw an exception when we call transformPoint(). 
To make sure we handle this gracefully, we'll catch the exception and print out an error for the user.*/

}

int main(int argc, char** argv){
  ros::init(argc, argv, "robot_tf_listener");
  ros::NodeHandle n;

  tf::TransformListener listener(ros::Duration(10));

  //we'll transform a point once every second
  ros::Timer timer = n.createTimer(ros::Duration(1.0), boost::bind(&transformPoint, boost::ref(listener)));

  ros::spin();

}