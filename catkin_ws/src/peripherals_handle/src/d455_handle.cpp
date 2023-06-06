#include "peripherals_handle/peripherals_handle_lib.h"
#include "/home/pino/pino_ws/papi/PAPI.h"

PAPI::communication::Server d455_server(DEFAULT_CAM_FORWARD_NODE_PORT);

bool image_exist = false;

std::string image_path = "/home/pino/image/d455_image.png";

void D455Callback(const sensor_msgs::Image::ConstPtr &msg)
{
    std::cout << "D455 Camera status: " << D455_exist(msg) << std::endl;

    const sensor_msgs::Image &ros_image = *msg;

    if (!image_exist)
    {
        image_exist = true;

        // Assuming 'ros_image' is the sensor_msgs::Image message
        cv_bridge::CvImagePtr cv_ptr;

        try
        {
            cv_ptr = cv_bridge::toCvCopy(ros_image, sensor_msgs::image_encodings::BGR8);
        }
        catch (cv_bridge::Exception &e)
        {
            ROS_ERROR("cv_bridge exception: %s", e.what());
            return;
        }

        cv::Mat cv_image = cv_ptr->image;
        if (cv::imwrite(image_path, cv_image))
        {
            std::cout << "Write image into " << image_path << std::endl;
        }
        else
        {
            std::cerr << "Cannot write image to " << image_path << std::endl;
        }
    }

    std::stringstream ss;
    ss << "CAM_FORWARD " << D455_exist(msg) << std::endl;
    d455_server.sendMsg(ss.str());
}

int connectToSercureService()
{
    return d455_server.serverStart();
}

int main(int argc, char **argv)
{
    if (connectToSercureService() == -1)
    {
        std::cerr << "Cannot connect to sercure service, kill d455_handle node.\n";
        return -1;
    }

    ros::init(argc, argv, "d455_handle");
    ros::NodeHandle nh;

    ros::Subscriber sub = nh.subscribe("/d400/color/image_raw", 10, D455Callback);

    ros::spin();

    /************************/

    d455_server.serverClose();

    return 0;
}