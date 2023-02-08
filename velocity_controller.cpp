/*
    controller.cpp
    Purpose: used for control the pendulum hardware

    @author Amit Ruidas
    @version 0.3 13/09/22
*/
#include "ros/ros.h"
#include <signal.h>
#include "pc7/Velocity.h"
#include "wrap.h" // the head of epos control functions
#include <cstdlib>
#include<time.h>

void mySigintHandler(int sig)
{
  ROS_INFO("server shutdown.");
  ros::shutdown();
}

int main(int argc, char **argv)
{

	int lResult = MMC_FAILED;
	unsigned int ulErrorCode = 0;
	long target;
	

	ros::init(argc, argv, "velocity_controller");
	ros::NodeHandle n;
	//ros::Rate r(1); // 1 hz
	
	ros::ServiceClient client = n.serviceClient<pc7::Velocity>("applyVelocity");

        pc7::Velocity srv;
	srv.request.velocity_new = 500;

	/*while(true)
	{
	cout << "Enter Forward_Velocity_Degree you want: ";
	cin >> srv.request.velocity_new;   
	//srv.request.velocity_new = atoi(argv[1])*/

        if (client.call(srv))
        {
                ROS_INFO("New Velocity: %ld", srv.response.velocity_old);
        }
        else
        {
                ROS_ERROR("Failed to call velocity service");
                return 1;
        }

	signal(SIGINT, mySigintHandler);
	return 0;
	}

	//ros::spin();
        //ros::spinOnce();
	//r.sleep();
	

