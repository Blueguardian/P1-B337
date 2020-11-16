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

    int width_room = 500;
    int length_room = 700;
    int nexhib = 0;

    std::cout << "Welcome user. How many exhibition points exist in the room?" << std::endl;
    std::cin >> nexhib;

    int array[nexhib][2] = {};

    for (int i = 0; i < nexhib; i++)
    { //This forloop loops through rows

        //for(int k = 0; k<2; k++){  //This forloop loops through columns

        int x = 0, y = 0, exhibwidth = 0, exhiblength = 0;

        std::cout << "what is the x coordinate? (cm) for the " << (i + 1);
        std::cout << "th point?" << std::endl;
        std::cin >> x;

        while (0 > x || x > length_room)
        {
            std::cout << "exhibiton is too big for room dum dum. Please enter a new value for x" << std::endl;
            std::cin >> x;
        }

        std::cout << "What is the y coordinate? (cm) " << (i + 1);
        std::cout << "th point?" << std::endl;
        std::cin >> y;

        while (0 > y || y > width_room)
        {
            std::cout << "The value for y exceeds the width of the room, dum dum. Please enter a new value for y" << std::endl;
            std::cin >> y;
        }

        
        array[i][0] = {x};
        array[i][1] = {y};

        //}
    }

    for (int p = 0; p < nexhib; p++) // Iterate over rows
    {
        for (int o = 0; o < 2; o++) // Iterate over elements within the row
        {
            std::cout << std::setw(12) << array[p][o];
        }
        std::cout << std::endl; // A new line for a new row
    }





    






    ros::NodeHandle nh;

    ros::service::waitForService("/turtle1/teleport_absolute", -1);

    ros::ServiceClient teleport_client = nh.serviceClient<turtlesim::TeleportAbsolute>("/turtle1/teleport_absolute");
    ros::ServiceClient pen_client = nh.serviceClient<turtlesim::SetPen>("/turtle1/set_pen");

    ros::Duration(2.0).sleep();

    turtlesim::SetPen pen_srv;
    pen_srv.request.off = true;
    pen_client.call(pen_srv);

    turtlesim::TeleportAbsolute srv;

    srv.request.x = 0.0;
    srv.request.y = 0.0;
    teleport_client.call(srv);
    pen_srv.request.off = false;
    pen_srv.request.width = 10;
    pen_client.call(pen_srv);

    srv.request.x = 0;
    srv.request.y = width_room; //neeeeeeejjjjjjjjjjjj
    teleport_client.call(srv);

    srv.request.x = length_room;
    srv.request.y = width_room;
    teleport_client.call(srv);

    srv.request.x = length_room;
    srv.request.y = 0;
    teleport_client.call(srv);

    srv.request.x = 0;
    srv.request.y = 0;
    teleport_client.call(srv);

    pen_srv.request.off = true;
    pen_client.call(pen_srv);

    for (int u = 0; u < nexhib; u++) // Iterate over rows
    {
        pen_srv.request.off = true;
        srv.request.x = array[u][0];
        srv.request.y = array[u][1];
        teleport_client.call(srv);

        pen_srv.request.off = false;
    }

    return 0;
}
