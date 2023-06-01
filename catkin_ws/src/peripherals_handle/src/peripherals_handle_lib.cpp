#include "peripherals_handle/peripherals_handle_lib.h"

int image_exist(const sensor_msgs::Image &_image)
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