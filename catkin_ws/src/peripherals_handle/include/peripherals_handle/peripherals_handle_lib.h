#include <iostream>
#include <string>

enum CAM_STATUS : int
{
    WAITING_FOR_ACTIVE = -1,
    ACTIVE,
    INACTIVE
};

ros::Time lastImageTime;
ros::Time firstImageTime;

int image_exist(const sensor_msgs::Image &_image);