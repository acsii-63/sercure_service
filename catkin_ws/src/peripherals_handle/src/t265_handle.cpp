// #include <ros/ros.h>
// #include <nav_msgs/Odometry.h>
// #include <geometry_msgs/PoseWithCovariance.h>
// #include <geometry_msgs/Pose.h>
// #include <geometry_msgs/Point.h>

#include "/home/pino/pino_ws/papi/PAPI.h"
#include "peripherals_handle/peripherals_handle_lib.h"

#include <iostream>
#include <string>

void odomCallback(const nav_msgs::Odometry::ConstPtr &msg)
{

}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "t265_handle");
    ros::NodeHandle nh;

    ros::Subscriber sub = nh.subscribe("/t265/odom/sample", 10, odomCallback);

    ros::spin();

    return 0;
}