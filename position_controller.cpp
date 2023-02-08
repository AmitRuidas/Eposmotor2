/*
    controller.cpp
    Purpose: used for control the pendulum hardware

    @author Amit Ruidas
    @version 0.3 13/09/22
*/
#include "ros/ros.h"
#include <signal.h>
#include "pc7/Position.h"
#include "wrap3.h" // the head of epos control functions
#include <cstdlib>
#include<time.h>

void mySigintHandler(int sig)
{
  ROS_INFO("controller shutdown.");
  ros::shutdown();
}
int main(int argc, char **argv)
{
	int lResult = MMC_FAILED;
	unsigned int ulErrorCode = 0;
	long target;
	char response;
	

	ros::init(argc, argv, "position_controller");
	ros::NodeHandle n;
	//ros::Rate r(1); // 1 hz
       
	ros::ServiceClient client = n.serviceClient<pc7::Position>("applyPosition");
        //ros::Publisher pub = n.advertise<pc7::Position>("applyPosition");
 	pc7::Position srv;

	srv.request.position_new = 50000;

	/*while(true)
	{
	cout << "Enter Position_Value you want: ";
	cin >> target; 	
	//target = atoi(argv[1])					  
	srv.request.position_new = target;
	//srv.request.position_new = atoi(argv[1])*/
			
	if (client.call(srv))
        {
		ROS_INFO("New Position: %ld", srv.response.position_old);
	}
	else
        {
                ROS_ERROR("Failed to call position service");
                return 1;
        }        
	signal(SIGINT, mySigintHandler);
	return 0;
	}
	
	//ros::spin();
	
	//ros::spinOnce();
	//r.sleep();
	
	
	 

	
