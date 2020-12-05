#include <ros/ros.h>
#include <geometry_msgs/PointStamped.h>
#include <actionlib/client/simple_client_goal_state.h>
#include <stdlib.h>
#include <time.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <std_msgs/Bool.h>
#include <std_msgs/Float32MultiArray.h>
#include <visualization_msgs/Marker.h>
#include <visualization_msgs/MarkerArray.h>
#include <tf2/LinearMath/Quaternion.h>

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

ros::NodeHandle *nhptr;
double coordx = 0;
double coordy = 0;
double coordz = 0;

void user_interface_cb(const std_msgs::Float32MultiArray::ConstPtr &msg);                                                        //Prints messeges containing the received coordinates                                                             //Prints messeges containing the received coordinates
void goal_reached_cb(const actionlib::SimpleClientGoalState &state, const move_base_msgs::MoveBaseResult::ConstPtr &result); //Goal has been reached                                                             //Odometry callback function
void send_goal(const geometry_msgs::PointStamped &goal_point);                                                                //Send goal to move_base server
void send_markers(move_base_msgs::MoveBaseGoal goal);

int main(int argc, char **argv)
{
    ros::init(argc, argv, "master"); //ros initialisation
    ros::NodeHandle nh2;
    nhptr = &nh2;

    ros::Rate loop(1);

    ros::Subscriber user_input = nh2.subscribe("user_input", 1, user_interface_cb);    // Subscribes to the user_input topic and when it receives a messege it runs the callback function
    ros::Publisher base_state_pub = nh2.advertise<std_msgs::Bool>("base_state", 5); //Creating a publisher for publishing the state of the MoveBaseClient    
}

void send_goal(const move_base_msgs::MoveBaseGoal &goal_point)
{
  MoveBaseClient ac("move_base", true);
  move_base_msgs::MoveBaseGoal goal;
  tf2::Quaternion rotation;
  rotation.setRPY(goal_point.target_pose.pose.orientation.x, goal_point.target_pose.pose.orientation.y, goal_point.target_pose.pose.orientation.z);
  rotation.normalize();
  goal.target_pose.pose.position = goal_point.target_pose.pose.position;
  goal.target_pose.pose.orientation.x = rotation.x();
  goal.target_pose.pose.orientation.y = rotation.y();
  goal.target_pose.pose.orientation.z = rotation.z();
  goal.target_pose.pose.orientation.w = rotation.w();
  ac.sendGoal(goal, boost::bind(&goal_reached_cb, _1, _2));
  send_markers(goal);
  ROS_INFO("Sending goal.."); //For testing and visualization purposes
  ac.waitForResult();
}

void send_markers(move_base_msgs::MoveBaseGoal goal)
{
  ros::Publisher marker_pub;
  marker_pub = nhptr->advertise<visualization_msgs::MarkerArray>("visualization_marker", 1);
  visualization_msgs::Marker marker;
  marker.header.stamp = ros::Time::now();
  marker.ns = "target_point";
  marker.type = visualization_msgs::Marker::ARROW;
  marker.action = visualization_msgs::Marker::ADD;
  marker.scale.x = 1.0;
  marker.scale.y = 0.2;
  marker.scale.z = 0.2;
  marker.color.r = 0.0;
  marker.color.g = 1.0;
  marker.color.b = 0.0;
  marker.color.a = 1.0;
  marker.pose.orientation.x = 0;
  marker.pose.orientation.y = 0.7071;
  marker.pose.orientation.z = 0;
  marker.pose.orientation.w = 0.7071;
  marker.lifetime = ros::Duration();
  marker.header.frame_id = goal.target_pose.header.frame_id;
  marker.id = 1;
  marker.pose.position = goal.target_pose.pose.position;
  marker.pose.position.z += marker.scale.x;
  ROS_INFO("Sending marker"); //For testing purposes.
  marker_pub.publish(marker);
}