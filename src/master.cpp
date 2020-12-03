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

ros::NodeHandle *ptrnh;

bool base_state = 0;
typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;
double coordx = 0;
double coordy = 0;
double coordz = 0;
void move_to_coord1(const std_msgs::Float32MultiArray::ConstPtr &msg);                                                        //Prints messeges containing the received coordinates                                                             //Prints messeges containing the received coordinates
void _goal_reached_cb(const actionlib::SimpleClientGoalState &state, const move_base_msgs::MoveBaseResult::ConstPtr &result); //Goal has been reached                                                             //Odometry callback function
void send_goal(const geometry_msgs::PointStamped &goal_point);                                                                //Send goal to move_base server
void send_markers(move_base_msgs::MoveBaseGoal goal);                                                                         //Send markers to the visualization software

int main(int argc, char **argv)
{
  ros::init(argc, argv, "master"); //ros initialisation
  ros::NodeHandle nh2;
  ros::Rate loop(1);
  ptrnh = &nh2;
  ros::Subscriber user_input = nh2.subscribe("user_input", 1, move_to_coord1);    // Subscribes to the user_input topic and when it receives a messege it runs the callback function
  ros::Publisher base_state_pub = nh2.advertise<std_msgs::Bool>("base_state", 5); //Creating a publisher for publishing the state of the MoveBaseClient

  MoveBaseClient ac("move_base", true); //Defining a client to send goals to the move_base server.
  //while (!ac.waitForServer(ros::Duration(5.0)))
  //{                                                                 //wait for the action server to come up
  //  ROS_INFO("Waiting for the move_base action server to come up"); //Printing a fitting messege.
  //}

  while (ros::ok()) //while(!= ros::Shutdown(); or the user has Ctrl+C out of the program.)
  {
    while ((coordx > 0.01 || coordx < -0.01) && (coordy > 0.01 || coordy < -0.01))
    {
        geometry_msgs::PointStamped goal; //Creates a new Goal of type MoveBaseGoal, for sending information to the move_base.

        ROS_INFO("Stored coordinates stored: [x: %f, y: %f, z: %f]", coordx, coordy, coordz);

        goal.header.stamp = ros::Time::now(); //Giving the information a time stamp
        goal.header.frame_id = "odom";   //Giving the information a frame to work with

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

void _goal_reached_cb(const actionlib::SimpleClientGoalState &state, const move_base_msgs::MoveBaseResult::ConstPtr &result)
{
  if (state == state.SUCCEEDED)
  {
    ROS_INFO("The goal has succesfully been reached!");
    ros::Publisher take_picture = ptrnh->advertise<std_msgs::Bool>("take_picture", 1);
    base_state = false;
    std_msgs::Bool msg;
    msg.data = true;
    take_picture.publish(msg);
    msg.data = false;
    take_picture.publish(msg);
  }
  else if (state == state.ACTIVE)
  {
    ROS_INFO("The robot is currently en route!");
    base_state = true;
  }
  else if (state == state.ABORTED)
  {
    ROS_INFO("The goal has been aborted!");
    base_state = false;
  }
  else if (state == state.LOST)
  {
    ROS_INFO("The goal has been lost!");
    base_state = false;
  }
  else if (state == state.PENDING)
  {
    ROS_INFO("The goal is currently pending!");
    base_state = true;
  }
  else if (state == state.REJECTED)
  {
    ROS_INFO("The goal has been rejected!");
    base_state = false;
  }
  else if (state == state.RECALLED)
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

void move_to_coord1(const std_msgs::Float32MultiArray::ConstPtr &msg)
{
  coordx = msg->data[0];
  coordy = msg->data[1];
  coordz = msg->data[2];
  ROS_INFO("Stored coordinates stored: [x: %f, y: %f, z: %f]", coordx, coordy, coordz); //For testing and errorhandling
}

void send_goal(const geometry_msgs::PointStamped &goal_point)
{
  MoveBaseClient ac("move_base", true);
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
  ROS_INFO("Sending goal.."); //For testing and visualization purposes
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
  marker.pose.position.z += marker.scale.x;
  marker_array.markers.push_back(marker);
  ROS_INFO("Sending marker"); //For testing purposes.
  marker_pub.publish(marker_array);
}
