#include <ros/ros.h>

#include <wfov_camera_msgs/WFOVImage.h>

#include <sensor_msgs/Image.h>

#include <nav_msgs/Odometry.h>
#include <geometry_msgs/PoseWithCovariance.h>
#include <geometry_msgs/Pose.h>
#include <geometry_msgs/Point.h>

#include <opencv2/opencv.hpp>
#include <cv_bridge/cv_bridge.h>

ros::Time lastFLIRTime;
ros::Time firstFLIRTime;

ros::Time firstD455Time;
ros::Time lastD455Time;

ros::Time lastLIDARTime;
ros::Time firstLIDARTime;

ros::Time lastT265Time;
ros::Time firstT265Time;

ros::Time lastFCUTime;
ros::Time firstFCUTime;

int FLIR_exist(const wfov_camera_msgs::WFOVImage::ConstPtr &msg);

int D455_exist(const sensor_msgs::Image::ConstPtr &msg);

int T265_exist(const nav_msgs::Odometry::ConstPtr &msg);