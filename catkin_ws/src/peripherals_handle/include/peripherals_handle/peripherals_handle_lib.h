#include <ros/ros.h>

#include <wfov_camera_msgs/WFOVImage.h>

#include <sensor_msgs/Image.h>

#include <nav_msgs/Odometry.h>
#include <geometry_msgs/PoseWithCovariance.h>
#include <geometry_msgs/Pose.h>
#include <geometry_msgs/Point.h>

#include <iostream>
#include <string>

#include <csignal>

ros::Time lastImageTime;
ros::Time firstImageTime;

// Return status of a image, exist or not.
int image_exist(const sensor_msgs::Image &_image);