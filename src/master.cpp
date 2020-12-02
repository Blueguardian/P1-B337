#include <algorithm>
#include <ros/ros.h>
#include <geometry_msgs/PointStamped.h>
#include <geometry_msgs/PoseStamped.h>
#include <actionlib/client/simple_client_goal_state.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <std_msgs/Bool.h>
#include <std_msgs/Float32.h>
#include <nav_msgs/Odometry.h>
#include <visualization_msgs/Marker.h>
#include <visualization_msgs/MarkerArray.h>
#include <tf2/LinearMath/Quaternion.h>


ros::NodeHandle* ptrnh;

bool base_state = 0;
bool base_fail;

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;
double coordx;
double coordy;
double coordz;
void move_to_coord1(const std_msgs::Float32::ConstPtr& msgx); //Prints messeges containing the received coordinates
void move_to_coord2(const std_msgs::Float32::ConstPtr& msgy); //Prints messeges containing the received coordinates
void move_to_coord3(const std_msgs::Float32::ConstPtr& msgz); //Prints messeges containing the received coordinates
void _goal_reached_cb(const actionlib::SimpleClientGoalState& state, const move_base_msgs::MoveBaseResult::ConstPtr& result); //Goal has been reached
void odom_callback(const nav_msgs::Odometry::ConstPtr& odom_msg); //Odometry callback function
void send_goal(const geometry_msgs::PointStamped& goal_point); //Send goal to move_base server
void send_markers(move_base_msgs::MoveBaseGoal goal); //Send markers to the visualization software


int main(int argc, char** argv){
  ros::init(argc, argv, "master"); //ros initialisation
  ros::NodeHandle nh2; //Testing nodehandle naming convention
  ptrnh = &nh2;

  MoveBaseClient ac("move_base", true); //Defining a client to send goals to the move_base server.

  //This needs to run for the program to succesfully contact the move_base
  //The movebase server needs the following to start: A static map or a running gmapping node, a functioning amcl setup, to be run
while(!ac.waitForServer(ros::Duration(5.0))){ //wait for the action server to come up
    ROS_INFO("Waiting for the move_base action server to come up"); //Printing a fitting messege.
  }
  ros::Subscriber user_input123 = nh2.subscribe("user_input1", 1, move_to_coord1); // Subscribes to the user_input topic and when it receives a messege it runs the callback function
  ros::Subscriber user_input321 = nh2.subscribe("user_input2", 1, move_to_coord2); // Subscribes to the user_input topic and when it receives a messege it runs the callback function
  ros::Subscriber user_input213 = nh2.subscribe("user_input3", 1, move_to_coord3); // Subscribes to the user_input topic and when it receives a messege it runs the callback function
  ros::Publisher base_state_pub = nh2.advertise<std_msgs::Bool>("base_state", 5); //Creating a publisher for publishing the state of the MoveBaseClient
  ros::Subscriber odom = nh2.subscribe<nav_msgs::Odometry>("/odom", 10, odom_callback);
  
  std::cout<<"x-coordinate stored:"<<coordx<<std::endl;  //Printing out the messege content that we copied
  std::cout<<"y-coordinate stored:"<<coordy<<std::endl;
  std::cout<<"z-coordinate stored:"<<coordz<<std::endl;

  ros::Rate loop(1);

while(ros::ok()) //while(!= ros::Shutdown(); or the user has Ctrl+C out of the program.)
  {
  while((coordx > 0.01 || coordx < -0.01) && (coordy > 0.01 || coordy < -0.01))
  {
  geometry_msgs::PointStamped goal; //Creates a new Goal of type MoveBaseGoal, for sending information to the move_base.

  std::cout<<"x-coordinate stored:"<<coordx<<std::endl;  //Printing out the messege content that we copied
  std::cout<<"y-coordinate stored:"<<coordy<<std::endl;
  std::cout<<"z-coordinate stored:"<<coordz<<std::endl;

  goal.header.stamp = ros::Time::now(); //Giving the information a time stamp
  goal.header.frame_id = "base_link"; //Giving the information a frame to work with (Will in the future be frames from the sensor)

  //we'll send a goal to the robot from ther user input 
  goal.point.x = coordx;
  goal.point.y = coordy;
  goal.point.z = coordz;  

  send_goal(goal);

  std_msgs::Bool state_get;
  state_get.data = base_state;

  base_state_pub.publish(state_get);

  loop.sleep(); 
  }
  ros::spinOnce();
  }
 return 0; //Program run succesfully.
}

void _goal_reached_cb(const actionlib::SimpleClientGoalState& state, const move_base_msgs::MoveBaseResult::ConstPtr& result)
{
  if(state == state.SUCCEEDED)
  {
    ROS_INFO("The goal has succesfully been reached!");
    ros::Publisher take_picture = ptrnh->advertise<std_msgs::Bool>("take_picture", 1);
    base_state = false;
    base_fail = false;
    std_msgs::Bool msg;
    msg.data = true;
    take_picture.publish(msg);
  }
  else if(state == state.ACTIVE)
  {
    ROS_INFO("The robot is currently en route!");
    base_state = true;
    base_fail = false;
  }
  else if(state == state.ABORTED)
  {
    ROS_INFO("The goal has been aborted!");
    base_state = false;
    base_fail = true;
  }
  else if(state == state.LOST)
  {
    ROS_INFO("The goal has been lost!");
    base_state = false;
    base_fail = true;
  }
  else if(state == state.PENDING)
  {
    ROS_INFO("The goal is currently pending!");
    base_state = true;
    base_fail = false;
  }
  else if (state == state.REJECTED)
  {
    ROS_INFO("The goal has been rejected!");
    base_state = false;
    base_fail = true;
  }
  else if(state == state.RECALLED)
  {
    ROS_INFO("The goal has been recalled!");
    base_state = false;
    base_fail = true;
  }
  else
  {
    ROS_INFO("Something went wrong!");
    base_state = false;
    base_fail = true;
  }
  
  
}

void move_to_coord1(const std_msgs::Float32::ConstPtr& msgx)
{
   coordx = msgx->data;
   std::cout<<"x-coord received:"<<coordx<<std::endl; //For testing and errorhandling
}

void move_to_coord2(const std_msgs::Float32::ConstPtr& msgy)
{
   coordy = msgy->data;
   std::cout<<"y-coord received:"<<coordy<<std::endl; //For testing and errorhandling
}

void move_to_coord3(const std_msgs::Float32::ConstPtr& msgz)
{
   coordz = msgz->data;
   std::cout<<"z-coord received:"<<coordz<<std::endl; //For testing and errorhandling
}

void odom_callback(const nav_msgs::Odometry::ConstPtr& odom_msg)
{
/*    nav_msgs::Odometry odom;

  odom.pose.pose.position.x = odom_msg->pose.pose.position.x;
  odom.pose.pose.position.y = odom_msg->pose.pose.position.y;
  odom.pose.pose.position.z = odom_msg->pose.pose.position.z;
  odom.pose.pose.orientation.x = odom_msg->pose.pose.orientation.x;
  odom.pose.pose.orientation.y = odom_msg->pose.pose.orientation.y;
  odom.pose.pose.orientation.z = odom_msg->pose.pose.orientation.z;
  odom.pose.pose.orientation.w = odom_msg->pose.pose.orientation.w;
  odom.twist.twist.linear.x = odom_msg->twist.twist.linear.x;
  odom.twist.twist.linear.y = odom_msg->twist.twist.linear.y;
  odom.twist.twist.linear.z = odom_msg->twist.twist.linear.z;
  odom.twist.twist.angular.x = odom_msg->twist.twist.angular.x;
  odom.twist.twist.angular.y = odom_msg->twist.twist.angular.y;
  odom.twist.twist.angular.z = odom_msg->twist.twist.angular.z;

  std::cout << "Current position: [x: " << odom_msg->pose.pose.position.x << ", y: " << odom_msg->pose.pose.position.y << "] \n";
  std::cout << "Current orientation: [x: " << odom_msg->pose.pose.orientation.x << ", y: " << odom_msg->pose.pose.orientation.y << ", z: " << odom_msg->pose.pose.orientation.z << ", w: " << odom_msg->pose.pose.orientation.w << "] \n";
  std::cout << "Current velocity: [Linear: x: " << odom_msg->twist.twist.linear.x << ", y: " << odom_msg->twist.twist.linear.y << ", z: " << odom_msg->twist.twist.linear.z << "] \n";
  std::cout << "[Angular: x: " << odom_msg->twist.twist.angular.x << ", y: " << odom_msg->twist.twist.angular.y << ", z: " << odom_msg->twist.twist.angular.z << "] \n";
*/
} 

void send_goal(const geometry_msgs::PointStamped& goal_point)
{
  MoveBaseClient ac("move_base", true);
  ros::Publisher take_picture = ptrnh->advertise<std_msgs::Bool>("take_picture", 1);
  move_base_msgs::MoveBaseGoal goal;
  tf2::Quaternion rotation;
  rotation.setRPY(0, 0, goal_point.point.z);
  rotation.normalize();
  goal.target_pose.header.frame_id = goal_point.header.frame_id;
  goal.target_pose.pose.position.x = goal_point.point.x;
  goal.target_pose.pose.position.y = goal_point.point.y;
  // goal.target_pose.pose.position.z = 1; //Unknown if needed
  goal.target_pose.pose.orientation.x = rotation.x();
  goal.target_pose.pose.orientation.y = rotation.y();
  goal.target_pose.pose.orientation.z = rotation.z();
  goal.target_pose.pose.orientation.w = rotation.w();
  ac.sendGoal(goal, boost::bind(&_goal_reached_cb, _1, _2));
  send_markers(goal);
  std::cout << "Sending goal.." << std::endl;
  ac.waitForResult();
  
}

 void send_markers(move_base_msgs::MoveBaseGoal goal)
{
    ros::Publisher marker_pub;
    marker_pub = ptrnh->advertise<visualization_msgs::MarkerArray>("busroute_markers", 1);
    visualization_msgs::Marker marker;
    visualization_msgs::MarkerArray marker_array;
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
    marker.pose.orientation.w = -0.7071;
    marker_array.markers.push_back(marker);
    marker_pub.publish(marker_array);
    }
    