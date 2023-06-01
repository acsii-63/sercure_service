#include <ros/ros.h>

#include <wfov_camera_msgs/WFOVImage.h>
#include <sensor_msgs/Image.h>
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/PoseWithCovariance.h>
#include <geometry_msgs/Pose.h>
#include <geometry_msgs/Point.h>

#include <iostream>
#include <string>

#define DEFAULT_CAM_FORWARD_NODE_PORT 24001  // D455
#define DEFAULT_CAM_DOWNWARD_NODE_PORT 24002 // Flir
#define DEFAULT_CAM_ODOM_NODE_PORT 24003     // T265
#define DEFAULT_LIDAR_NODE_PORT 24004        // Lidar
#define DEAULT_FCU_NODE_PORT 24005           // FCU?

enum CAM_STATUS : int
{
    WAITING_FOR_ACTIVE = -1,
    ACTIVE,
    INACTIVE
};

ros::Time lastImageTime;
ros::Time firstImageTime;

// Return status of a image, exist or not.
int image_exist(const sensor_msgs::Image &_image);