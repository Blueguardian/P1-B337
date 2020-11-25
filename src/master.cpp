#include <algorithm>
#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
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


bool base_state;

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;
double coordx;
double coordy;
double coordz;
MoveBaseClient ac("move_base", true);


void move_to_coord1(const std_msgs::Float32::ConstPtr& msgx) //Prints messeges containing the received coordinates
{
   coordx = msgx->data;
   std::cout<<"x-coord received:"<<coordx<<std::endl; //For testing and errorhandling
  }

  void move_to_coord2(const std_msgs::Float32::ConstPtr& msgy) //Prints messeges containing the received coordinates
{
   coordy = msgy->data;
   std::cout<<"y-coord received:"<<coordy<<std::endl; //For testing and errorhandling
  }
  void move_to_coord3(const std_msgs::Float32::ConstPtr& msgz) //Prints messeges containing the received coordinates
{
   coordz = msgz->data;
   std::cout<<"z-coord received:"<<coordz<<std::endl; //For testing and errorhandling
  }
void _goal_reached_cb(const actionlib::SimpleClientGoalState& state, const move_base_msgs::MoveBaseResult::ConstPtr& result)
{
  if(state == state.SUCCEEDED)
  {
    ROS_INFO("The goal has succesfully been reached!");
    base_state = false;
  }
  else if(state == state.ACTIVE)
  {
    ROS_INFO("The robot is currently en route!");
    base_state = true;
  }
  else if(state == state.ABORTED)
  {
    ROS_INFO("The goal has been aborted!");
    base_state = false;
  }
  else if(state == state.LOST)
  {
    ROS_INFO("The goal has been lost!");
    base_state = false;
  }
  else if(state == state.PENDING)
  {
    ROS_INFO("The goal is currently pending!");
    base_state = true;
  }
  else if (state == state.REJECTED)
  {
    ROS_INFO("The goal has been rejected!");
    base_state = false;
  }
  else if(state == state.RECALLED)
  {
    ROS_INFO("The goal has been recalled!");
    base_state = false;
  }
  else
  {
    ROS_INFO("Something went wrong!");
    base_state = false;
  }
  
  
}

void odom_callback(const nav_msgs::Odometry::ConstPtr& odom_msg) //Odometry callback function
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
  move_base_msgs::MoveBaseGoal goal;
  goal.target_pose.header.frame_id = goal_point.header.frame_id;
  goal.target_pose.pose.position.x = goal_point.point.x;
  goal.target_pose.pose.position.y = goal_point.point.y;
  goal.target_pose.pose.position.z = 1;
  goal.target_pose.pose.orientation.w = goal_point.point.z;
  ac.sendGoal(goal, boost::bind(&_goal_reached_cb, _1, _2));
  std::cout << "Sending goal.." << std::endl;
}

int main(int argc, char** argv){
  ros::init(argc, argv, "master"); //ros initialisation
  ros::NodeHandle nh2; //Testing nodehandle naming convention

  MoveBaseClient ac("move_base", true); //Defining a client to send goals to the move_base server.

  //This needs to run for the program to succesfully contact the move_base
  //The movebase server needs the following to start: A static map or a running gmapping node, a functioning amcl setup, to be run
 /* while(!ac.waitForServer(ros::Duration(5.0))){ //wait for the action server to come up
    ROS_INFO("Waiting for the move_base action server to come up"); //Printing a fitting messege.
  } */
  ros::Subscriber user_input123 = nh2.subscribe("user_input1", 1, move_to_coord1); // Subscribes to the user_input topic and when it receives a messege it runs the callback function
  ros::Subscriber user_input321 = nh2.subscribe("user_input2", 1, move_to_coord2); // Subscribes to the user_input topic and when it receives a messege it runs the callback function
  ros::Subscriber user_input213 = nh2.subscribe("user_input3", 1, move_to_coord3); // Subscribes to the user_input topic and when it receives a messege it runs the callback function

  std::cout<<"x-coordinate stored:"<<coordx<<std::endl;  //Printing out the messege content that we copied
  std::cout<<"y-coordinate stored:"<<coordy<<std::endl;
  std::cout<<"z-coordinate stored:"<<coordz<<std::endl;

  ros::Rate loop(10);

while(ros::ok()) //while(!= ros::Shutdown(); or the user has Ctrl+C out of the program.)
  {
  geometry_msgs::PointStamped goal; //Creates a new Goal of type MoveBaseGoal, for sending information to the move_base.

  std::cout<<"x-coordinate stored:"<<coordx<<std::endl;  //Printing out the messege content that we copied
  std::cout<<"y-coordinate stored:"<<coordy<<std::endl;
  std::cout<<"z-coordinate stored:"<<coordz<<std::endl;

  goal.header.stamp = ros::Time::now(); //Giving the information a time stamp
  goal.header.frame_id = "base_link"; //Giving the information a frame to work with (Will in the future be frames from the sensor)

  //we'll send a goal to the robot from ther user input // Omitted until a loop for all coordinates have been implementet
  goal.point.x = coordx;
  goal.point.y = coordy;
  goal.point.z = coordz;  

  ros::Publisher base_state_pub = nh2.advertise<std_msgs::Bool>("base_state", 5); //Creating a publisher for publishing the state of the MoveBaseClient
  ros::Subscriber odom = nh2.subscribe<nav_msgs::Odometry>("/odom", 10, odom_callback);


  std_msgs::Bool state_get;
  state_get.data = base_state;

  base_state_pub.publish(state_get);

  send_goal(goal);

  ac.waitForResult();

  loop.sleep(); 
  ros::spin();
}

 if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
   ROS_INFO("Hooray, the base moved 1 meter forward");
 else
  ROS_INFO("The base failed to move forward 1 meter for some reason");

 return 0; //Program run succesfully.
}