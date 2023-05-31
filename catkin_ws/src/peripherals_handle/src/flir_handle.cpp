#include <ros/ros.h>
#include <wfov_camera_msgs/WFOVImage.h>
#include <sensor_msgs/Image.h>

#include "/home/pino/pino_ws/papi/PAPI.h"
#include "peripherals_handle/peripherals_handle_lib.h"

#include <iostream>
#include <string>

void imageCallback(const wfov_camera_msgs::WFOVImage::ConstPtr &msg)
{
    const sensor_msgs::Image &image = msg->image;

    std::cout << "FLIR Camera status: " << cam_status(image) << std::endl;
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "flir_handle");
    ros::NodeHandle nh;

    ros::Subscriber sub = nh.subscribe("/camera/image", 10, imageCallback);

    ros::spin();

    return 0;
}