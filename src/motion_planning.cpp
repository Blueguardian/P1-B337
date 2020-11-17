#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>

geometry_msgs::Twist motion_planning_msg;


struct roomType
{
    int room_length;
    int room_width;
    int num_exhibits;
};

roomType insertRoom()
{
    roomType newRoomType;
    std::cout << "Welcome operator! \n You are currently operating the Museum X-300 scanner robot. \n Please input the room length and width in meters to continue. \n";
    std::cout << "Room length:";
    std::cin >> newRoomType.room_length;
    while(newRoomType.room_length < 0)
    {
        std::cout << "Incorrect value, please try again \n Room length:";
        std::cin >> newRoomType.room_length;
    }
    std::cout << "Room width:";
    std::cin >> newRoomType.room_width;
    while(newRoomType.room_width < 0)
    {
        std::cout << "Incorrect value, please try again \n Room width:";
        std::cin >> newRoomType.room_width;
    }
    std::cout << "The room dimensions are: Length: " << newRoomType.room_length << "m Width: " << newRoomType.room_width << "m. \n";
    std::cout << "Please input the number of exhibitions present in the room:";
    std::cin >> newRoomType.num_exhibits;
    return newRoomType;
}

void insertCoord(int (*array)[2], int room_length, int room_width, int numexhi)
{
    int x, y;
    std::cout << "The selected room is " << room_length << " meters long and " << room_width << " meters wide.";
    for(int i=0; i<numexhi;i++)
    {
        std::cout << "Please input the length of the x-coordinate in cm for the " << i << "th exhibit:";
        std::cin >> x;
        array[i][0] = x;
        while(x < 0)
        {
            std::cout << "Incorrect value, please try again \n Length of x-coordinate in cm for the " << i << "th exhibit:";
            std::cin >> x;
            array[i][0] = x;
        }
        std::cout << "Please input the length of the y-coordinate in cm for the " << i << "th exhibit:";
        std::cin >> y;
        array[i][1] = y;
        while(y < 0)
        {
            std::cout << "Incorrect value, please try again \n Length of y-coordinate in cm for the " << i << "th exhibit:";
            std::cin >> y;
            array[i][1] = y;
        }
    }
}

void sortCoord(int (*array)[2], int startpos, )
{
    
}

int main(int argc, char *argv[])
{
    ros::init(argc, argv, "motion_planning");
    ros::NodeHandle nh1;

    ros::Publisher motion_planning_pub = nh1.advertise<geometry_msgs::Twist>("motion_planning", 1);

    int coordarray[][2] = {};
    roomType room;
    room = insertRoom();









    return 0;
}
