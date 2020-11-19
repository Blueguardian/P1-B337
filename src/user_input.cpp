#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <std_msgs/Float32.h>

bool base_state;

struct roomType         //Creating a new datatype called roomType
{
    double room_length; //roomType variable called room_length of type double
    double room_width;  //roomType variable called room_width of type double
    int num_exhibits; //roomType variable called num_exhibits of type int
};

roomType insertRoom() //roomType function called insertRoom for user input.
{
    //The function asks the user to input the data for each variable to fill out the type. 
    //It asks the user for input for each variable in the type roomType, and the function then
    //fills out each variable with the users input.

    //start of function

    roomType newRoomType; //creating the roomType data type
    std::cout << "Welcome operator! \n You are currently operating the Museum X-300 scanner robot. \n Please input the room length and width in meters to continue. \n";
    std::cout << "Room length:";
    std::cin >> newRoomType.room_length; //assigning the user input to the variable room_length.
    while(newRoomType.room_length <= 0)
    {
        std::cout << "Incorrect value, please try again \n Room length:";
        std::cin >> newRoomType.room_length;
    }
    std::cout << "Room width:";
    std::cin >> newRoomType.room_width;
    while(newRoomType.room_width <= 0)
    {
        std::cout << "Incorrect value, please try again \n Room width:";
        std::cin >> newRoomType.room_width;
    }
    std::cout << "The room dimensions are: Length: " << newRoomType.room_length << "m Width: " << newRoomType.room_width << "m. \n";
    std::cout << "Please input the number of exhibitions present in the room:";
    std::cin >> newRoomType.num_exhibits;
    while(newRoomType.num_exhibits <= 0)
    {
        ROS_INFO("There must be at least one exhibit, and the exhibit number cannot be negative! Please try again! \n Please input the number of exhibitions present in the room: ");
        std::cin >> newRoomType.num_exhibits;
    }
    return newRoomType;
}

void base_state_get(const std_msgs::Bool::ConstPtr& msg)
{
    ROS_INFO("Base processing coordinates.. Moving..")
    base_state = *msg;
}

double euclidianDist(double x1, double y1)
{
    double dist = pow(x1, 2)+pow(y1, 2);
    dist = sqrt(dist);
    return dist;
}

void insertCoord(double (*array)[2], double room_length, double room_width, int numexhi)
{
    double x, y;
    std::cout << "The selected room is " << room_length << " meters long and " << room_width << " meters wide.";
    for(int i=0; i<numexhi;i++)
    {
        std::cout << "Please input the length of the x-coordinate in meters for the " << i << "th exhibit:";
        std::cin >> x;
        array[i][0] = x;
        while(x < 0)
        {
            std::cout << "Incorrect value, please try again \n Length of x-coordinate in meters for the " << i << "th exhibit:";
            std::cin >> x;
            array[i][0] = x;
        }
        std::cout << "Please input the length of the y-coordinate in meters for the " << i << "th exhibit:";
        std::cin >> y;
        array[i][1] = y;
        while(y < 0)
        {
            std::cout << "Incorrect value, please try again \n Length of y-coordinate in meters for the " << i << "th exhibit:";
            std::cin >> y;
            array[i][1] = y;
        }
    }
}

void sortCoord(double (*array)[2], int startpos, int itera)
{
    double temp1;
    double temp2;
    for(int i = startpos; i<itera; i++)
    {
        for(int j = i+1; j<itera; j++)
        {
            if((euclidianDist(array[i][0], array[i][1])) > (euclidianDist(array[j][0], array[j][1])))
            {
                temp1 = array[i][0];
                temp2 = array[i][0];
                array[i][0] = array[j][0];
                array[i][1] = array[j][1];
                array[j][0] = temp1;
                array[j][1] = temp2;
            }
        }
    }
} 

int main(int argc, char *argv[])
{
    ros::init(argc, argv, "user_input");
    ros::NodeHandle nh1;

    ros::Publisher user_input_pub = nh1.advertise<x_300_master::coord>("user_input", 1);
  
    roomType room;
    room = insertRoom();
    double coordarray[room.num_exhibits][2];
    insertCoord(coordarray, room.room_length, room.room_width, room.num_exhibits);

    for(int i = 0; i < room.num_exhibits; i++)
    {
            std::cout << "Unsorted coordset: [" << coordarray[i][0] << ", " << coordarray[i][1] << "] \n";
    }

    sortCoord(coordarray, 0, room.num_exhibits);

    for(int i = 0; i < room.num_exhibits; i++)
    {
            std::cout << "Sorted coordset: [" << coordarray[i][0] << ", " << coordarray[i][1] << "] \n";
    }
    while(ros::ok())
    {
  
        ros::Subscriber base_state = nh1.subscribe("base_state", 5, )
        x_300_master::coord coord;
//        double x_begincoord = coordarray[0][0];
//        double y_begincoord = coordarray[0][1];


//        coord.coordx = x_begincoord;
//        coord.coordy = y_begincoord;

        while(base_state.Data == 0 && i != room.num_exhibits)
            {
                int i = 0;
                sortCoord(coordarray, i, room.num_exhibits);
                double x_coord = coordarray[i][0];
                double y_coord = coordarray[i][1];

                coord.coordx = x_coord;
                coord.coordy = y_coord;
                i++;

                user_input_pub.publish(coord);

                loop.sleep();

                ros::spinOnce();
            }


    }

    return 0;
}
