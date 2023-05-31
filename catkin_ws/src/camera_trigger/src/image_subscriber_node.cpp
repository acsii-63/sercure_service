#include <ros/ros.h>
#include <wfov_camera_msgs/WFOVImage.h>
#include <sensor_msgs/Image.h>

#include "/home/pino/pino_ws/papi/PAPI.h"

#include <iostream>
#include <string>

// void imageCallback(const wfov_camera_msgs::WFOVImage::ConstPtr &msg)
// {
//     const sensor_msgs::Image &image = msg->image;

//     if (image.data.size() > 0)
//     {
//         ROS_INFO("Received an image with width: %d, height: %d", image.width, image.height);
//         // Perform operations on the image
//     }
//     else
//     {
//         ROS_WARN("Received an empty image");
//     }
// }

ros::Time lastImageTime;
ros::Time firstImageTime;

enum CAM_STATUS : int
{
    WAITING_FOR_ACTIVE = -1,
    ACTIVE,
    INACTIVE
};

int cam_status(const sensor_msgs::Image &_image)
{
    if (_image.data.size() > 0)
    {
        // First callback loop
        if (firstImageTime.toSec() <= 0)
            firstImageTime = ros::Time::now();

        lastImageTime = ros::Time::now();
    }
    else
    {
        ros::Duration timeSinceLastImage = ros::Time::now() - lastImageTime;
        if (timeSinceLastImage.toSec() > 1.0)
            return CAM_STATUS::INACTIVE;
    }

    ros::Duration timeSinceFirstImage = ros::Time::now() - firstImageTime;
    if (timeSinceFirstImage.toSec() > 5.0)
        return CAM_STATUS::ACTIVE;
    else
        return CAM_STATUS::WAITING_FOR_ACTIVE;
}

void imageCallback(const wfov_camera_msgs::WFOVImage::ConstPtr &msg)
{
    const sensor_msgs::Image &image = msg->image;

    std::cout << "Cam status: " << cam_status(image) << std::endl;

    
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "image_subscriber_node");
    ros::NodeHandle nh;

    ros::Subscriber sub = nh.subscribe("/camera/image", 10, imageCallback);

    ros::spin();

    return 0;
}