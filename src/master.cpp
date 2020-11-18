#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

void move_to_coord(move_base_msgs::MoveBaseAction ac("", true), const ::x_300_master::coord::ConstPtr& msg)
{
  move_base_msgs::MoveBaseGoal goal;
  goal.target_pose.header.stamp = ros::Time::now();
  goal.target_pose.header.frame_id = "base_link";

  goal.target_pose.pose.position.x = msg->x;
  goal.target_pose.pose.position.y = msgy->y;
}

int main(int argc, char** argv){
  ros::init(argc, argv, "simple_navigation_goals");

  //Define a client to send goals to the move_base server.
  move_base_msgs::MoveBaseAction ac("move_base", true);

  //wait for the action server to come up
  while(!ac.waitForServer(ros::Duration(5.0))){
    ROS_INFO("Waiting for the move_base action server to come up");
  }
    
  

  //we'll send a goal to the robot to move 1 meter forward
  
  



  ROS_INFO("Sending goal");
  ac.sendGoal(goal);

  ac.waitForResult();

  if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
    ROS_INFO("Hooray, the base moved 1 meter forward");
  else
    ROS_INFO("The base failed to move forward 1 meter for some reason");

  return 0;
}