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

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;
void userInterface_cb(const geometry_msgs::PointStamped::ConstPtr &msg);                                                        //Prints messeges containing the received coordinates                                                             //Prints messeges containing the received coordinates
void goal_reached_cb(const actionlib::SimpleClientGoalState &state, const move_base_msgs::MoveBaseResult::ConstPtr &result); //Goal has been reached                                                             //Odometry callback function
void send_goal(move_base_msgs::MoveBaseGoal goal_point);                                                                //Send goal to move_base server
void send_marker(move_base_msgs::MoveBaseGoal goal); 
double rob_facing_angle(double angle);
move_base_msgs::MoveBaseGoal get_dif2Dgoal(move_base_msgs::MoveBaseGoal goal);                                                                

int main(int argc, char **argv)
{
  ros::init(argc, argv, "master"); //ros initialisation
  ros::NodeHandle nh2;
  ros::Rate loop(1);
  ptrnh = &nh2;
  ros::Subscriber user_input = nh2.subscribe("point_clicked", 1, userInterface_cb);    // Subscribes to the user_input topic and when it receives a messege it runs the callback function
  ros::Publisher base_state_pub = nh2.advertise<std_msgs::Bool>("base_state", 5); //Creating a publisher for publishing the state of the MoveBaseClient

  MoveBaseClient ac("move_base", true); //Defining a client to send goals to the move_base server.
  while (!ac.waitForServer(ros::Duration(5.0)))
  {                                                                 //wait for the action server to come up
    ROS_INFO("Waiting for the move_base action server to come up"); //Printing a fitting messege.
  }

  while (ros::ok()) //while(!= ros::Shutdown(); or the user has Ctrl+C out of the program.)
  {

  }
  return 0; //Program run succesfully.
}

void _goal_reached_cb(const actionlib::SimpleClientGoalState &state, const move_base_msgs::MoveBaseResult::ConstPtr &result)
{
  if (state == state.SUCCEEDED)
  {
    ROS_INFO("The goal has succesfully been reached!");
    ros::Publisher take_picture = ptrnh->advertise<std_msgs::Bool>("take_picture", 1);
    std_msgs::Bool msg;
    msg.data = true;
    take_picture.publish(msg);
    ros::Rate sleep(0.7);
    sleep.sleep();
    msg.data = false;
    take_picture.publish(msg);
  }
  else if (state == state.ACTIVE)
  {
    ROS_INFO("The robot is currently en route!");
  }
  else if (state == state.ABORTED)
  {
    ROS_INFO("The goal has been aborted!");
  }
  else if (state == state.LOST)
  {
    ROS_INFO("The goal has been lost!");
  }
  else if (state == state.PENDING)
  {
    ROS_INFO("The goal is currently pending!");
  }
  else if (state == state.REJECTED)
  {
    ROS_INFO("The goal has been rejected!");
  }
  else if (state == state.RECALLED)
  {
    ROS_INFO("The goal has been recalled!");
  }
  else
  {
    ROS_INFO("Something went wrong!");
  }
}

void userInterface_cb(const geometry_msgs::PointStamped::ConstPtr &msg)
{
  ROS_INFO("Stored coordinates: [x: %f, y: %f, z: %f]", msg->point.x, msg->point.x, msg->point.z); //For testing and errorhandling
  move_base_msgs::MoveBaseGoal goal_target;
  tf2::Quaternion rotation;

  ROS_INFO("Calculating goal position.."); //Testing purposes

  goal_target.target_pose.pose.position.x = msg->point.x;
  goal_target.target_pose.pose.position.y = msg->point.y;
  goal_target.target_pose.pose.orientation.z = msg->point.z;

  goal_target = get_dif2Dgoal(goal_target);

  rotation.setRPY(0, 0, rob_facing_angle(msg->point.z));
  rotation.normalize();

  goal_target.target_pose.pose.orientation.x = rotation.x();
  goal_target.target_pose.pose.orientation.y = rotation.y();
  goal_target.target_pose.pose.orientation.z = rotation.z();
  goal_target.target_pose.pose.orientation.w = rotation.w();

  goal_target.target_pose.header.frame_id = msg->header.frame_id;
  send_goal(goal_target);
}

void send_goal(move_base_msgs::MoveBaseGoal goal_point)
{
  MoveBaseClient ac("move_base", true);
  ac.sendGoal(goal_point, boost::bind(&_goal_reached_cb, _1, _2));
  ROS_INFO("Sending goal..");
  send_marker(goal_point);

  ac.waitForResult();
}

double rob_facing_angle(double angle)
{
    //This function takes the angle orientation of the particular object and converts it into
    //an angle that would points directly towards the exhibition.

    //beginning of the function

    int oppositeangle = 0;

    if(angle>=0 && angle<=M_PI) //If this is true, the angle of the exhibitions would be added to Pi, to face it with a positive angle
        { 
        oppositeangle = angle+M_PI;
        }
    else if(angle>M_PI && angle<(2*M_PI)) //If this is true, the angle of the exhibitions would be added to Pi, to face it with a positive angle
        { 
        oppositeangle = angle-M_PI;
        } 
    return oppositeangle;
}

void send_marker(move_base_msgs::MoveBaseGoal goal)
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

move_base_msgs::MoveBaseGoal get_dif2Dgoal(move_base_msgs::MoveBaseGoal goal)
{
    double dif_x = 0.5*cos(goal.target_pose.pose.orientation.z);
    double dif_y = 0.5*sin(goal.target_pose.pose.orientation.z);

    move_base_msgs::MoveBaseGoal goal_target;
    goal_target.target_pose.pose.orientation.z = goal.target_pose.pose.orientation.z;
    goal_target.target_pose.pose.position.x = goal.target_pose.pose.position.x-dif_x;
    goal_target.target_pose.pose.position.y = goal.target_pose.pose.position.y-dif_y;

    return goal_target;
}
