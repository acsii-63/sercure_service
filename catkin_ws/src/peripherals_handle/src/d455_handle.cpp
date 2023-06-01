// #include <ros/ros.h>
// #include <sensor_msgs/Image.h>

#include "/home/pino/pino_ws/papi/PAPI.h"
#include "peripherals_handle/peripherals_handle_lib.h"

#include <iostream>
#include <string>

PAPI::communication::Server d455_server(DEFAULT_CAM_FORWARD_NODE_PORT);

void imageCallback(const sensor_msgs::Image::ConstPtr &_image)
{
    std::cout << "D455 Camera status: " << image_exist(*_image) << std::endl;

    std::stringstream ss;
    ss << "CAM_FORWARD " << image_exist(*_image) << std::endl;
    d455_server.sendMsg(ss.str());
}

void init()
{
    d455_server.serverStart();
}

int main(int argc, char **argv)
{   
    init();

    ros::init(argc, argv, "d455_handle");
    ros::NodeHandle nh;

    ros::Subscriber sub = nh.subscribe("/d400/color/image_raw", 10, imageCallback);

    ros::spin();
    
    /************************/

    d455_server.serverClose();

    return 0;
}