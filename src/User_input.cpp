#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <turtlesim/TeleportAbsolute.h>
#include <turtlesim/SetPen.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>

//THIS COMMENT IS A TEST


int main(int argc, char *argv[])
{
    ros::init(argc, argv, "move_turtle");

    //int width_room = 500;
    //int length_room = 700; Currently not necessary - Used to be dimensions for the room

    int nexhib = 0, robx = 0, roby = 0, x = 0, y = 0;
    int robotpos[2] ={};

    std::cout << "Welcome user. How many exhibition points exist in the room?" << std::endl;
    std::cin >> nexhib;

    int array[nexhib][2] = {};

    for (int i = 0; i < nexhib; i++)
    { //This forloop loops through rows

        //for(int k = 0; k<2; k++){  //This forloop loops through columns

        

        std::cout << "what is the x coordinate? (cm) for the " << (i + 1);
        std::cout << "th point?" << std::endl;
        std::cin >> x;

        while (0 > x)
        {
            std::cout << "exhibiton is too big for room dum dum. Please enter a new value for x" << std::endl;
            std::cin >> x;
        }

        std::cout << "What is the y coordinate? (cm) " << (i + 1);
        std::cout << "th point?" << std::endl;
        std::cin >> y;

        while (0 > y)
        {
            std::cout << "The value for y exceeds the width of the room, dum dum. Please enter a new value for y" << std::endl;
            std::cin >> y;
        }

        
        array[i][0] = {x};
        array[i][1] = {y};

        //}
    }
        std::cout << "\n";
        std::cout << "Great. Based on your exhibit coordinates you'll now have to enter the coordinates for where the robot will be positioned, according to the other exhibits (As unsigned integer):" << std::endl;
        std::cout << "\n";
        std::cout << "Enter x-coordinate for robot position" << std::endl;
        std::cin >> robx;

        while (0 > robx)
        {
            std::cout << "The entered x-coordinate for the robot was negative. Try again" << std::endl;
            std::cin >> robx;
        }

        std::cout << "Enter Y-coordinate for robot position" << std::endl;
        std::cin >> roby;

        while (0 > roby)
        {
            std::cout << "The entered y-coordinate for the robot was negative. Try again" << std::endl;
            std::cin >> roby;
        }


//The following code isn't a part of the pseudo-code. With this we just check the stored user input
    for (int p = 0; p < nexhib; p++) // Iterate over rows
    {
        for (int o = 0; o < 2; o++) // Iterate over elements within the row
        {
            std::cout << std::setw(12) << array[p][o];
        }
        std::cout << std::endl; // A new line for a new row
    }


    return 0;
}
