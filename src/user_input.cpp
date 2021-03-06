#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <stdlib.h>
#include <time.h>
#include <std_msgs/Bool.h>
#include <std_msgs/Float32MultiArray.h>
#include <tf/transform_listener.h>
#include <std_msgs/Empty.h>
#include <math.h>
#include <bits/stdc++.h>

bool base_state; //Global variable to store data from the master function call.

struct point
{
    double x = 0;
    double y = 0;
    double z = 0;
};

struct roomType //Creating a new datatype called roomType
{
    double room_length; 
    double room_width;  
    int num_exhibits; 
};

roomType insertRoom(double roomLength, double roomwidth); //roomType function called insertRoom for user input.
double rob_facing_angle(double angle); // Function for calculation of robot orientation
double euclidianDist(double x1, double y1, double refx, double refy); //Distance measuring function used for comparisson
void base_state_get(const std_msgs::Bool::ConstPtr& msg); //Callback function for base_state
void insertCoord(point (*array), double room_length, double room_width, int numexhi); //User input function for the coordinates for each exhibit.
void sortCoord(point (*array), int startpos, int itera, double refx, double refy);  //sorting function for the array from a point of reference
void degreeToRadian(point (*array), int itera); //Converter from degrees to radians

int main(int argc, char *argv[]) //main function
{
    ros::init(argc, argv, "user_input"); //initializing ros
    ros::NodeHandle nh1; //creating a nodehandle for the node.
    ros::Rate loop(0.1); //creating a loop rate for pauses (10 seconds)

    ros::Publisher reset_odom = nh1.advertise<std_msgs::Empty>("move_base/commands/reset_odometry", 1); //Creating a publisher for resetting the odometry
    ros::Publisher publish_point = nh1.advertise<std_msgs::Float32MultiArray>("user_input", 1); //creating a publisher for the user_input to publish it later
    ros::Subscriber base_state_sub = nh1.subscribe("base_state", 5, base_state_get); //Creating a subscriber to get the current state of the move_base //Needs to be looked over
    
    std_msgs::Empty odom_res;
    reset_odom.publish(odom_res); //Sending msg to reset odometry

    double roomwidth = 66*0.05; //Room dimensions based on the static map
    double roomlength = 156*0.05; //Room dimensions based on the static map

    roomType room; //creating a variable of type roomType
    room = insertRoom(roomlength, roomwidth); //asking the user for the dimensions of the room and the number of exhibits

    point point1;
    point1.x = 0;
    point1.y = 0;
    point1.z = 0;
    point coordarray[room.num_exhibits+1]; //defining an array of size  [room.num_exhibits][2] because it only moves in a 2 dimensional manner
    coordarray[0] = point1;

    insertCoord(coordarray, room.room_length, room.room_width, room.num_exhibits+1); //asks the user to input coordinates for each exhibit

    degreeToRadian(coordarray, room.num_exhibits+1); //Converting degrees to radians

    //For testing purposes
    for(int i = 0; i < room.num_exhibits+1; i++) //printing the unsorted array for testing purposes
    {
        ROS_INFO("Unsorted coordset: [x: %f, y: %f, z: %f]", coordarray[i].x, coordarray[i].y, coordarray[i].z);
    }

    sortCoord(coordarray, 0, room.num_exhibits+1, 0, 0); //sorts the array the first time

    //For testing purposes
    for(int i = 0; i < room.num_exhibits+1; i++) //printing the sorted array for testing purposes
    {
        ROS_INFO("Unsorted coordset: [x: %f, y: %f, z: %f]", coordarray[i].x, coordarray[i].y, coordarray[i].z);
    }

    while(ros::ok()) //Starting the ros loop
    {
        double x_coord = coordarray[0].x;
        double y_coord = coordarray[0].y;
        double z_coord = coordarray[0].z;

        int iter = 1; //create an iterator for the number of times the array needs to be sorted

        while(iter != room.num_exhibits) //While loop to keep looping until there are no more exhibits
        {   
           while(base_state == false) //While loop that only runs when the robot is finished with it's current task //Needs work
              {
                    sortCoord(coordarray, iter, room.num_exhibits+1, x_coord, y_coord); //Sorting the coordinate array again until all points have been processed

                    z_coord = coordarray[iter].z;

                    ROS_INFO("Coordset before change: [x: %f, y: %f, z: %f", coordarray[iter].x, coordarray[iter].y, coordarray[iter].z);

                    double dif_x = 0.5*cos(z_coord);
                    double dif_y = 0.5*sin(z_coord);

                    ROS_INFO("Differentials: [x_dif: %f, y_dif: %f]", dif_x, dif_y);

                    if(z_coord<=M_PI && z_coord>=0){
                    x_coord = coordarray[iter].x+dif_x; //Assigning the coordinates to variables
                    y_coord = coordarray[iter].y+dif_y;
                    }
                    else{    //The angle must be between Pi and 2*Pi
                    x_coord = (coordarray[iter].x-dif_x); //Assigning the coordinates to variables
                    y_coord = (coordarray[iter].y-dif_y);
                    }

                    z_coord = rob_facing_angle(z_coord);
                    ROS_INFO("Coordset before sending: [x: %f, y: %f, z: %f", coordarray[iter].x, coordarray[iter].y, coordarray[iter].z);

                    std_msgs::Float32MultiArray msgArray;
                    msgArray.data.resize(3);
                    msgArray.data[0] = x_coord;
                    msgArray.data[1] = y_coord;
                    msgArray.data[2] = z_coord;

                    iter++; //increment the iterator to let the program know, that the coordinateset has been processed and needs no further processing

                    ROS_INFO("Message data for transfer: [x: %f, y: %f, z: %f]", msgArray.data[0], msgArray.data[1], msgArray.data[2]);

                    publish_point.publish(msgArray); //Publish the next first coordinate
                    loop.sleep(); //Sleep for 10 milliseconds before trying again
                    base_state = true;
                }
            loop.sleep();    
        }   

    }
return 0; //Program ran succesfully
}

roomType insertRoom(double roomLength, double roomWidth) 
{
    //The function asks the user to input the value for each variable in roomType. 
    //The function then checks if the input is valid, if not then it will tell the
    //user that the input is valid, and let them try again, until the value is valid
    //then fills out each variable with the users input.

    //Room dimensions should be input from the comming map.yaml file

    //beginning of function insertRoom()

    roomType newRoomType; //creating the roomTy0pe data type
    ROS_INFO("Welcome operator! \n You are currently operating the Museum X-300 scanner robot. \n The current room has been scanned. \n");
    ROS_INFO("The room has the following dimensions: [Length: %f, Width: %f] \n", roomLength, roomWidth);
    ROS_INFO("Please input the number of exhibitions: ");
    std::cin >> newRoomType.num_exhibits;
    newRoomType.num_exhibits = static_cast<int> (newRoomType.num_exhibits);
    while(newRoomType.num_exhibits <= 0) //validation check
    {
        ROS_WARN("There must be at least one exhibit! Please try again! \n Please input the number of exhibitions present in the room: ");
        std::cin >> newRoomType.num_exhibits;
    }
    newRoomType.room_length = roomLength;
    newRoomType.room_width = roomWidth;
    return newRoomType;
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

double euclidianDist(double x1, double y1, double refx, double refy)
{
    //This function takes in two coordinates of type double (x1 and y1) and calculates the distance from a point of
    //reference (refx and refy) and returns the euclidian distance between these.

    //beginning of function

    double distx = pow(x1-refx, 2); //Reference distance calculation
    double disty = pow(y1-refy, 2); //Input distance calculation
    double dist = sqrt(distx+disty); //calculation of distance between reference point and input point
    return dist;
}

void base_state_get(const std_msgs::Bool::ConstPtr& msg)
{
    //The callback function is called when a messege is received from the base_state subscriber.
    //This callback function sends out a messege telling the user that the base is still moving
    //and assign a pointer to the global variable base_state to be able to use the variable later.

    // beginning of function

    if (msg->data == 1) //If the current goal has not been reached, wait.
        {
        ROS_INFO("Base processing coordinates.. Moving..");
        }
    else //If the current goal has succeded send new coordinates
        {
        ROS_INFO("Current job finished.. processing new goal..");
        }
    base_state = msg->data;
}

void insertCoord(point (*array), double room_length, double room_width, int numexhi)
{
    //The function tells the user the size of the room and takes the number of exhibits
    //and then asks the user to input the coordinates for each exhibit and checks if the coordinat is
    //within range of the room. This function assumes that the robot is in the middle of the room.

    //beginning of function

    int i=1;
    double x, y, z;
    while(i < numexhi)
    {
        ROS_INFO("Please input the length of the x-coordinate in meters for the %d. exhibit: ", i);
        std::cin >> x;
        array[i].x = x;
        while(x < -1*(room_length/2) || x > room_length/2) //validation check
        {
            ROS_WARN("Incorrect value, please try again! \nPlease input the length of the x-coordinate in meters for the %d. exhibit: ", i);
            std::cin >> x;
            array[i].x = x;
        }
        ROS_INFO("Please input the length of the y-coordinate in meters for the %d. exhibit: ", i);
        std::cin >> y;
        array[i].y = y;
        while(y < -1*(room_width/2) || y > room_width/2) //validation check
        {
            ROS_WARN("Incorrect value, please try again! \nPlease input the length of the y-coordinate in meters for the %d. exhibit: ", i);
            std::cin >> y;
            array[i].y = y;
        }
        ROS_INFO("Please input the orientation of the %d. exhibit in degrees", i);
        std::cin >> z;
        array[i].z = z;
        while(z > 360 || z < 0) //validation check
        {
            ROS_WARN("Incorrect value, please try again! \nPlease input the orientation of the %d. exhibit", i);
            std::cin >> z;
            array[i].z = z;
        }
        i++;
    }
}

void sortCoord(point (*array), int startpos, int itera, double refx, double refy)
{
    //The function takes an array, a starting position, a number of iterations, since it ensures that the array does not get too big,
    //and takes a set of coordinates for the point of reference, It then compares the array's coordinatesets by calling the euclidianDist() function
    //to compare them by their euclidian distance. It then switches the sets if the former set is smaller than the latter.

    //beginning of function

    for(int i = startpos; i<itera; i++) //iterator for the first coordinateset
    {
        for(int j = i+1; j<itera; j++) //iterator for the second coordinateset
        {
            if((euclidianDist(array[i].x, array[i].y, refx, refy) > (euclidianDist(array[j].x, array[j].y, refx, refy))))
            {
                //switches the places of the coordinateset if it's smaller.
                std::swap(array[i].x, array[j].x);
                std::swap(array[i].y, array[j].y);
                std::swap(array[i].z, array[j].z);

            }
        }
    }
} 

void degreeToRadian(point (*array), int itera)
{
    for(int i = 0; i < itera; i++) //Converting degress to radians in all elements
    {
        double temp;
        temp = array[i].z;
        temp = (M_PI/180)*temp;
        array[i].z = temp;
    }
}