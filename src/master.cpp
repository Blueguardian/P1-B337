#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <std_msgs/Bool.h>
#include <std_msgs/Float32.h>

bool base_state;

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;
double coordx;
double coordy;

void move_to_coord1(const std_msgs::Float32::ConstPtr& msg) //Prints messeges containing the received coordinates
{
   coordx = msg->data;
   std::cout<<"x-coord received:"<<coordx<<std::endl;
  }

  void move_to_coord2(const std_msgs::Float32::ConstPtr& msg) //Prints messeges containing the received coordinates
{
   coordy = msg->data;
   std::cout<<"y-coord received:"<<coordy<<std::endl;
  }

int main(int argc, char** argv){
  ros::init(argc, argv, "simple_navigation_goals"); //ros initialisation
  ros::NodeHandle nh2; //Testing nodehandle naming convention

  MoveBaseClient ac("move_base", true); //Defining a client to send goals to the move_base server.

  
  while(!ac.waitForServer(ros::Duration(5.0))){ //wait for the action server to come up
    ROS_INFO("Waiting for the move_base action server to come up"); //Printing a fitting messege.
  }
  ros::Subscriber user_input123 = nh2.subscribe("user_input1", 1, move_to_coord1); // Subscribes to the user_input topic and when it receives a messege it runs the callback function
  ros::Subscriber user_input321 = nh2.subscribe("user_input2", 1, move_to_coord2); // Subscribes to the user_input topic and when it receives a messege it runs the callback function

  move_base_msgs::MoveBaseGoal goal; //Creates a new Goal of type MoveBaseGoal, for sending information to the move_base.
  goal.target_pose.header.stamp = ros::Time::now(); //Giving the information a time stamp
  goal.target_pose.header.frame_id = "base_link"; //Giving the information a frame to work with (Will in the future be frames from the sensor)

//we'll send a goal to the robot from ther user input // Omitted until a loop for all coordinates have been implementet
// goal.target_pose.pose.position.x = coordinateSet.x;
// goal.target_pose.pose.position.y = coordinateSet.y;

std::cout<<"x-coordinate stored:"<<coordx<<std::endl;  //Printing out the messege content that we copied
std::cout<<"y-coordinate stored:"<<coordy<<std::endl;

ros::Publisher base_state_pub = nh2.advertise<std_msgs::Bool>("base_state", 5); //Creating a publisher for publishing the state of the MoveBaseClient

ros::Rate loop(10);
std_msgs::Bool msg_b;
while(ros::ok()) //while(!= ros::Shutdown(); or the user has Ctrl+C out of the program.)
{
  
  if(ac.getState() != actionlib::SimpleClientGoalState::ACTIVE) //As long as the current goal is active, don't send new coordinates from user_input
  {
    base_state = true; 
    msg_b.data = base_state;
    base_state_pub.publish(msg_b); //Publish base_state
  }
  else
  {
    base_state = false;
    msg_b.data = base_state;
    base_state_pub.publish(msg_b); //Publish base_state
  }
  

  loop.sleep(); 
}
//Omitted until coordinateSet is interchangeable over the subscriber.
//ac.sendGoal(goal);

// ac.waitForResult();

 if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
   ROS_INFO("Hooray, the base moved 1 meter forward");
 else
  ROS_INFO("The base failed to move forward 1 meter for some reason");

 return 0; //Program run succesfully.
}