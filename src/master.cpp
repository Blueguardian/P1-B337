#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

struct coordSet{
  double x;
  double y;
} coordinateSet;

void move_to_coord(const x_300_master::coord::ConstPtr& msg) //Printer beskeder om hvad der er modtaget af coordinater
{
  ROS_INFO(("x-coordinate received: ", msg.x))
  ROS_INFO(("y-coordinate received: ", msg.y))
  coordinateSet = *msg;
  }

int main(int argc, char** argv){
  ros::init(argc, argv, "simple_navigation_goals");
  ros::NodeHandle nh2;

  //Define a client to send goals to the move_base server.
  move_base_msgs::MoveBaseAction ac("move_base", true);

  //wait for the action server to come up
  while(!ac.waitForServer(ros::Duration(5.0))){
    ROS_INFO("Waiting for the move_base action server to come up");
  }
  ros::Subscriber user_input = n.subscribe("user_input", 1, coordCallback);
  
  move_base_msgs::MoveBaseGoal goal;
  goal.target_pose.header.stamp = ros::Time::now();
  goal.target_pose.header.frame_id = "base_link";

//we'll send a goal to the robot from ther user input // Omitted until a loop for all coordinates have been implementet
 // goal.target_pose.pose.position.x = coordinateSet.x;
// goal.target_pose.pose.position.y = coordinateSet.y;
  
ROS_INFO("x-coordinate stored: ", coordinateSet.x);
ROS_INFO("y-coordinate stored: ", coordinateSet.y)


ROS_INFO("Sending goal");

while(ros::ok())
{
  ros::Publisher base_state_pub = nh2.advertise<std_msgs::Bool>("base_state", 5);
}
//Omitted until coordinateSet is interchangeable over the subscriber.
//ac.sendGoal(goal);

// ac.waitForResult();

// if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
//   ROS_INFO("Hooray, the base moved 1 meter forward");
// else
//  ROS_INFO("The base failed to move forward 1 meter for some reason");

  return 0;
}