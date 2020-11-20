#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <std_msgs/Bool.h>
#include <P1-B337/coord.h>
#include <std_msgs/Float32.h>

std_msgs::Bool base_state;
typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;
double coordx;
double coordy;

void move_to_coord(const (P1-B337)::coord::ConstPtr& msg) //Prints messeges containing the received coordinates
{
  ROS_INFO(("x-coordinate received: %f", msg.x));
  ROS_INFO(("y-coordinate received: %f", msg.y));
  coordx = msg.coordx; //Copies the msg content
  coordy = msg.coordy; //Copies the msg content
  }

int main(int argc, char** argv){
  ros::init(argc, argv, "simple_navigation_goals"); //ros initialisation
  ros::NodeHandle nh2; //Testing nodehandle naming convention

  move_base_msgs::MoveBaseAction ac("move_base", true); //Defining a client to send goals to the move_base server.

  
  while(!ac.waitForServer(ros::Duration(5.0))){ //wait for the action server to come up
    ROS_INFO("Waiting for the move_base action server to come up"); //Printing a fitting messege.
  }
  ros::Subscriber user_input = n.subscribe("user_input", 1, move_to_coord); // Subscribes to the user_input topic and when it receives a messege it runs the callback function
  
  move_base_msgs::MoveBaseGoal goal; //Creates a new Goal of type MoveBaseGoal, for sending information to the move_base.
  goal.target_pose.header.stamp = ros::Time::now(); //Giving the information a time stamp
  goal.target_pose.header.frame_id = "base_link"; //Giving the information a frame to work with (Will in the future be frames from the sensor)

//we'll send a goal to the robot from ther user input // Omitted until a loop for all coordinates have been implementet
// goal.target_pose.pose.position.x = coordinateSet.x;
// goal.target_pose.pose.position.y = coordinateSet.y;
  
ROS_INFO("x-coordinate stored: %f", coordx); //Printing out the messege content that we copied
ROS_INFO("y-coordinate stored: %f", coordy);

ros::Rate loop(10)

ROS_INFO("Sending goal"); //Printing out a fitting messege:

while(ros::ok()) //while(!= ros::Shutdown(); or the user has Ctrl+C out of the program.)
{
  ros::Publisher base_state_pub = nh2.advertise<std_msgs::Bool>("base_state", 5); //Creating a publisher for publishing the state of the MoveBaseClient
  if(ac.getState() != actionlib::SimpleClientGoalState::ACTIVE) //As long as the current goal is active, don't send new coordinates from user_input
  {
    base_state = 1; 
    base_state_pub.Publish(base_state, 1) //Publish base_state
  }
  else
  {
    base_state = 0;
    base_state_pub.Publish(baste_state, 1) //Publish base_state
  }
  

  loop.rate(); 
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