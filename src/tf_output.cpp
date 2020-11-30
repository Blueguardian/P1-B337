#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
//The tf package provides an implementation of a tf::TransformBroadcaster to help make the task of publishing transforms easier. 
//To use the TransformBroadcaster, we need to include the tf/transform_broadcaster.h header file.

int main(int argc, char** argv){
  ros::init(argc, argv, "robot_tf_publisher");
  ros::NodeHandle n;

  ros::Rate r(100);

  tf::TransformBroadcaster broadcaster;
  //Here, we create a TransformBroadcaster object that we'll use later to send the base_link → base_laser transform over the wire.

  while(n.ok()){
    broadcaster.sendTransform(
      tf::StampedTransform(
        tf::Transform(tf::Quaternion(0, 0, 0, 1), tf::Vector3(0.1, 0.0, -0.165)), //Measured on real turtlebot
        ros::Time::now(),"base_link", "base_laser"));
    r.sleep();

    /*This is where the real work is done. Sending a transform with a TransformBroadcaster requires five arguments.
    First, we pass in the rotation transform, which is specified by a btQuaternion for any rotation that needs to occur between the two coordinate frames.
    In this case, we want to apply no rotation, so we send in a btQuaternion constructed from pitch, roll, and yaw values equal to zero. 
    Second, a btVector3 for any translation that we'd like to apply.
    We do, however, want to apply a translation, so we create a btVector3 corresponding to the laser's x offset of 10cm and z offset of 20cm from the robot base. 
    Third, we need to give the transform being published a timestamp, we'll just stamp it with ros::Time::now(). 
    Fourth, we need to pass the name of the parent node of the link we're creating, in this case "base_link.
    "Fifth, we need to pass the name of the child node of the link we're creating, in this case "base_laser." */
  }
}
