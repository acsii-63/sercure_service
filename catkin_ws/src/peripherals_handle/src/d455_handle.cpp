#include <ros/ros.h>
#include <sensor_msgs/Image.h>

#include "/home/pino/pino_ws/papi/PAPI.h"

#include <iostream>
#include <string>

void imageCallback(const sensor_msgs::Image::ConstPtr &msg)
{

}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "d455_handle");
    ros::NodeHandle nh;

    ros::Subscriber sub = nh.subscribe("/d400/color/image_raw", 10, imageCallback);

    ros::spin();

    return 0;
}