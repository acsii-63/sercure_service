#include "peripherals_handle/peripherals_handle_lib.h"
#include "/home/pino/pino_ws/papi/PAPI.h"

PAPI::communication::Server server(DEFAULT_CAM_DOWNWARD_NODE_PORT);

bool image_exist = false;

std::string image_path = "/home/pino/image/flir_image.png";

void FLIRCallback(const wfov_camera_msgs::WFOVImage::ConstPtr &msg)
{
    std::cout << "FLIR Camera status: " << FLIR_exist(msg) << std::endl;

    const sensor_msgs::Image &ros_image = msg->image;

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
    ss << "CAM_DOWNWARD " << FLIR_exist(msg) << std::endl;
    server.sendMsg(ss.str());
}

int connectToSercureService()
{
    return server.serverStart();
}

int main(int argc, char **argv)
{
    if (connectToSercureService() == -1)
    {
        std::cerr << "Cannot connect to sercure service, kill flir_handle node.\n";
        return -1;
    }

    ros::init(argc, argv, "flir_handle");
    ros::NodeHandle nh;

    ros::Subscriber sub = nh.subscribe("/camera/image", 10, FLIRCallback);

    ros::spin();

    /************************/

    server.serverClose();

    return 0;
}