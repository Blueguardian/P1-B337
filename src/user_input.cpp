#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <std_msgs/Float32.h>

bool base_state; //Global variable to store data from the master function call.

struct roomType         //Creating a new datatype called roomType
{
    double room_length; //roomType variable called room_length of type double
    double room_width;  //roomType variable called room_width of type double
    int num_exhibits; //roomType variable called num_exhibits of type int
};

roomType insertRoom() //roomType function called insertRoom for user input.
{
    //The function asks the user to input the value for each variable in roomType. 
    //The function then checks if the input is valid, if not then it will tell the
    //user that the input is valid, and let them try again, until the value is valid
    //then fills out each variable with the users input.

    //beginning of function insertRoom()

    roomType newRoomType; //creating the roomType data type
    std::cout << "Welcome operator! \n You are currently operating the Museum X-300 scanner robot. \n Please input the room length and width in meters to continue. \n";
    std::cout << "Room length:";
    std::cin >> newRoomType.room_length; 
    while(newRoomType.room_length <= 0) //validation check
    {
        std::cout << "Incorrect value, please try again \n Room length:"; 
        std::cin >> newRoomType.room_length; 
    }
    std::cout << "Room width:"; 
    std::cin >> newRoomType.room_width; 
    while(newRoomType.room_width <= 0) //validation check
    {
        std::cout << "Incorrect value, please try again \n Room width:"; 
        std::cin >> newRoomType.room_width;
    }
    std::cout << "The room dimensions are: Length: " << newRoomType.room_length << "m Width: " << newRoomType.room_width << "m. \n";
    std::cout << "Please input the number of exhibitions present in the room:";
    std::cin >> newRoomType.num_exhibits;
    while(newRoomType.num_exhibits <= 0) //validation check
    {
        ROS_INFO("There must be at least one exhibit, and the exhibit number cannot be negative! Please try again! \n Please input the number of exhibitions present in the room: ");
        std::cin >> newRoomType.num_exhibits;
    }
    return newRoomType;
}

void base_state_get(const std_msgs::Bool::ConstPtr& msg) //Callback function for base_state
{
    //The callback function is called when a messege is received from the base_state subscriber.
    //This callback function sends out a messege telling the user that the base is still moving
    //and assign a pointer to the global variable base_state to be able to use the variable later.

    // beginning of function

    ROS_INFO("Base processing coordinates.. Moving..")
    base_state = *msg;
}

double euclidianDist(double x1, double y1, double refx, double refy) //Distance measuring function used for comparisson
{

    //This function takes in two coordinates of type double (x1 and y1) and calculates the distance from a point of
    //reference (refx and refy) and returns the euclidian distance between these.

    //beginning of function

    double refdist = pow(refx, 2)+pow(refy, 2)
    double dist = pow(x1, 2)+pow(y1, 2);
    dist = sqrt(refdist-dist);
    return dist;
}

void insertCoord(double (*array)[2], double room_length, double room_width, int numexhi) //User input function for the coordinates for each exhibit.
{
    double x, y;
    std::cout << "The selected room is " << room_length << " meters long and " << room_width << " meters wide.";
    for(int i=0; i<numexhi;i++)
    {
        std::cout << "Please input the length of the x-coordinate in meters for the " << i << "th exhibit:";
        std::cin >> x;
        array[i][0] = x;
        while(x < -1*(room_length/2) && x > room_length/2)
        {
            std::cout << "Incorrect value, please try again \n Length of x-coordinate in meters for the " << i << "th exhibit:";
            std::cin >> x;
            array[i][0] = x;
        }
        std::cout << "Please input the length of the y-coordinate in meters for the " << i << "th exhibit:";
        std::cin >> y;
        array[i][1] = y;
        while(y < -1*(room_width/2) && y > room_width/2)
        {
            std::cout << "Incorrect value, please try again \n Length of y-coordinate in meters for the " << i << "th exhibit:";
            std::cin >> y;
            array[i][1] = y;
        }
    }
}

void sortCoord(double (*array)[2], int startpos, int itera, double refx, double refy)
{
    double temp1;
    double temp2;
    for(int i = startpos; i<itera; i++)
    {
        for(int j = i+1; j<itera; j++)
        {
            if((euclidianDist(array[i][0], array[i][1], refx, refy) > (euclidianDist(array[j][0], array[j][1], refx, refy)))
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

    sortCoord(coordarray, 0, room.num_exhibits, 0, 0);

    for(int i = 0; i < room.num_exhibits; i++)
    {
            std::cout << "Sorted coordset: [" << coordarray[i][0] << ", " << coordarray[i][1] << "] \n";
    }
    while(ros::ok())
    {
        x_300_master::coord coord;
        ros::Subscriber base_state = nh1.subscribe("base_state", 5, base_state_get)
        
//        double x_begincoord = coordarray[0][0];
//        double y_begincoord = coordarray[0][1];


//        coord.coordx = x_begincoord;
//        coord.coordy = y_begincoord;

        double x_coord = coordarray[0][0];
        double y_coord = coordarray[0][1];

        coord.coordx = x_coord;
        coord.coordy = y_coord;

        user_input_pub.publish(coord);
        int i = 1;

        while(i != room.num_exhibits)
        {   

            while(base_state.Data == 0)
                {

                    sortCoord(coordarray, i, room.num_exhibits, x_coord, y_coord);
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


    }

    return 0;
}
