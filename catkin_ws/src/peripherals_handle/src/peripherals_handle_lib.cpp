#include "peripherals_handle/peripherals_handle_lib.h"

#include "/home/pino/pino_ws/papi/PAPI.h"

int FLIR_exist(const wfov_camera_msgs::WFOVImage::ConstPtr &msg)
{
    const sensor_msgs::Image &image = msg->image;
    if (image.data.size() > 0)
    {
        // First callback loop
        if (firstFLIRTime.toSec() <= 0)
            firstFLIRTime = ros::Time::now();

        lastFLIRTime = ros::Time::now();
    }
    else
    {
        ros::Duration timeSinceLastImage = ros::Time::now() - lastFLIRTime;
        if (timeSinceLastImage.toSec() > 1.0)
            return PERIPHERAL_STATUS::INACTIVE;
    }

    ros::Duration timeSinceFirstImage = ros::Time::now() - firstFLIRTime;
    if (timeSinceFirstImage.toSec() > 5.0)
        return PERIPHERAL_STATUS::ACTIVE;
    else
        return PERIPHERAL_STATUS::WAITING_FOR_ACTIVE;
}

int D455_exist(const sensor_msgs::Image::ConstPtr &msg)
{
    const sensor_msgs::Image &image = *msg;
    if (image.data.size() > 0)
    {
        // First callback loop
        if (firstD455Time.toSec() <= 0)
            firstD455Time = ros::Time::now();

        lastD455Time = ros::Time::now();
    }
    else
    {
        ros::Duration timeSinceLastImage = ros::Time::now() - lastD455Time;
        if (timeSinceLastImage.toSec() > 1.0)
            return PERIPHERAL_STATUS::INACTIVE;
    }

    ros::Duration timeSinceFirstImage = ros::Time::now() - firstD455Time;
    if (timeSinceFirstImage.toSec() > 5.0)
        return PERIPHERAL_STATUS::ACTIVE;
    else
        return PERIPHERAL_STATUS::WAITING_FOR_ACTIVE;
}

int T265_exist(const nav_msgs::Odometry::ConstPtr &msg)
{
    geometry_msgs::PoseWithCovariance pose_with_convariance = msg->pose;
    // geometry_msgs::Pose pose = pose_with_convariance.pose;
    // geometry_msgs::Point point = pose.position;
    geometry_msgs::Point point = msg->pose.pose.position;

    if (point.x != 0 && point.y != 0 && point.z != 0)
    {
        // First callback loop
        if (firstT265Time.toSec() <= 0)
            firstT265Time = ros::Time::now();

        lastT265Time = ros::Time::now();
    }
    else
    {
        ros::Duration timeSinceLastImage = ros::Time::now() - lastT265Time;
        if (timeSinceLastImage.toSec() > 1.0)
            return PERIPHERAL_STATUS::INACTIVE;
    }

    ros::Duration timeSinceFirstImage = ros::Time::now() - firstT265Time;
    if (timeSinceFirstImage.toSec() > 5.0)
        return PERIPHERAL_STATUS::ACTIVE;
    else
        return PERIPHERAL_STATUS::WAITING_FOR_ACTIVE;
}