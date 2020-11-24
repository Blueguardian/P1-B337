#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <std_msgs/Bool.h>
#include <std_msgs/Float32.h>
#include <tf/transform_listener.h>
#include <std_msgs/Empty.h>



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

    roomType newRoomType; //creating the roomTy0pe data type
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

    ROS_INFO("Base processing coordinates.. Moving..");
    base_state = msg->data;
}

double euclidianDist(double x1, double y1, double refx, double refy) //Distance measuring function used for comparisson
{

    //This function takes in two coordinates of type double (x1 and y1) and calculates the distance from a point of
    //reference (refx and refy) and returns the euclidian distance between these.

    //beginning of function

    double refdist = pow(refx, 2)+pow(refy, 2);
    double dist = pow(x1, 2)+pow(y1, 2);
    dist = sqrt(dist-refdist);
    return dist;
}

void insertCoord(double (*array)[2], double room_length, double room_width, int numexhi) //User input function for the coordinates for each exhibit.
{
    //The function tells the user the size of the room and takes the number of exhibits
    //and then asks the user to input the coordinates for each exhibit and checks if the coordinat is
    //within range of the room. This function assumes that the robot is in the middle of the room.

    //beginning of function
    double x, y;
    std::cout << "The selected room is " << room_length << " meters long and " << room_width << " meters wide.";
    for(int i=0; i<numexhi;i++) //for each exhibit
    {
        std::cout << "Please input the length of the x-coordinate in meters for the " << i << "th exhibit:";
        std::cin >> x;
        array[i][0] = x;
        while(x < -1*(room_length/2) && x > room_length/2) //validation check
        {
            std::cout << "Incorrect value, please try again \n Length of x-coordinate in meters for the " << i << "th exhibit:";
            std::cin >> x;
            array[i][0] = x;
        }
        std::cout << "Please input the length of the y-coordinate in meters for the " << i << "th exhibit:";
        std::cin >> y;
        array[i][1] = y;
        while(y < -1*(room_width/2) && y > room_width/2) //validation check
        {
            std::cout << "Incorrect value, please try again \n Length of y-coordinate in meters for the " << i << "th exhibit:";
            std::cin >> y;
            array[i][1] = y;
        }
    }
}

void sortCoord(double (*array)[2], int startpos, int itera, double refx, double refy) //sorting function for the array from a point of reference
{
    //The function takes an array, a starting position, a number of iterations, since it ensures that the array does not get too big,
    //and takes a set of coordinates for the point of reference, it then compares the array's coordinatesets by calling the euclidianDist() function
    //to compare them by their euclidian distance. It then switches the sets if the former set is smaller than the latter.

    //beginning of function

    double temp1;
    double temp2;
    for(int i = startpos; i<itera; i++) //iterator for the first coordinateset
    {
        for(int j = i+1; j<itera; j++) //iterator for the second coordinateset
        {
            if((euclidianDist(array[i][0], array[i][1], refx, refy) > (euclidianDist(array[j][0], array[j][1], refx, refy))))
            {
                //switches the places of the coordinateset if it's smaller.
                temp1 = array[i][0];
                temp2 = array[i][1];
                array[i][0] = array[j][0];
                array[i][1] = array[j][1];
                array[j][0] = temp1;
                array[j][1] = temp2;
            }
        }
    }
} 

int main(int argc, char *argv[]) //main function
{
    ros::init(argc, argv, "user_input"); //initializing ros
    ros::NodeHandle nh1; //creating a nodehandle for the node.

    ros::Publisher reset_odom = nh1.advertise<std_msgs::Empty>("move_base/commands/reset_odometry", 1);
    std_msgs::Empty odom_res;

    reset_odom.publish(odom_res);

    tf::TransformListener tf_transform;

    ros::Publisher publish_x = nh1.advertise<std_msgs::Float32>("user_input1", 1); //creating a publisher for the user_input to publish it later
    ros::Publisher publish_y = nh1.advertise<std_msgs::Float32>("user_input2", 1); //creating a publisher for the user_input to publish it later


    roomType room; //creating a variable of type roomType
    room = insertRoom(); //asking the user for the dimensions of the room and the number of exhibits

    double coordarray[room.num_exhibits][2]; //defining an array of size  [room.num_exhibits][2] because it only moves in a 2 dimensional manner
    insertCoord(coordarray, room.room_length, room.room_width, room.num_exhibits); //asks the user to input coordinates for each exhibit

    ros::Rate loop(50); //creating a loop rate for pauses (10 milliseconds)

    for(int i = 0; i < room.num_exhibits; i++) //printing the unsorted array for testing purposes
    {
            std::cout << "Unsorted coordset: [" << coordarray[i][0] << ", " << coordarray[i][1] << "] \n";
    }

    sortCoord(coordarray, 0, room.num_exhibits, 0, 0); //sorts the array the first time

    for(int i = 0; i < room.num_exhibits; i++) //printing the sorted array for testing purposes
    {
            std::cout << "Sorted coordset: [" << coordarray[i][0] << ", " << coordarray[i][1] << "] \n";
    }
    while(ros::ok()) //Starting the ros loop
    {
        ros::Subscriber base_state = nh1.subscribe("base_state", 5, base_state_get); //Creating a subscriber to get the current state of the move_base //Needs to be looked over

        double x_coord = coordarray[0][0]; //assigning the first set of coordinates to variables
        double y_coord = coordarray[0][1];

        std_msgs::Float32 msg_x;
        std_msgs::Float32 msg_y;

        msg_x.data = y_coord; 
        msg_y.data = x_coord;

        publish_x.publish(msg_x); //Publish the first coordinate
        publish_y.publish(msg_y); //Publish the second coordinate
        int iter = 1; //create an iterator for the number of times the array needs to be sorted

        while(iter != room.num_exhibits) //While loop to keep looping until there are no more exhibits
        {   

           while(base_state == false) //While loop that only runs when the robot is finished with it's current task //Needs work
               {

                    sortCoord(coordarray, iter, room.num_exhibits, x_coord, y_coord); //Sorting the coordinate array again until all points have been processed
                    double x_coord = coordarray[iter][0]; //Assigning the coordinates to variables
                    double y_coord = coordarray[iter][1];

                    std_msgs::Float32 msg_x;
                    std_msgs::Float32 msg_y;

                    msg_x.data = x_coord; //assigning the coordinates to the messege.
                    msg_y.data = y_coord;
                    iter++; //increment the iterator to let the program know, that the coordinateset has been processed and needs no further processing

                    publish_x.publish(msg_x); //Publish the next first coordinate
                    publish_y.publish(msg_y); //Publish the next second coordinate

                    loop.sleep(); //Sleep for 10 milliseconds before trying again

                }    
        }


    }

    return 0; //Program ran succesfully
}

