#include "peripherals_handle/peripherals_handle_lib.h"
#include "/home/pino/pino_ws/papi/PAPI.h"

PAPI::communication::Server d455_server(DEFAULT_CAM_FORWARD_NODE_PORT);

void D455Callback(const sensor_msgs::Image::ConstPtr &msg)
{
    std::cout << "D455 Camera status: " << D455_exist(msg) << std::endl;

    std::stringstream ss;
    ss << "CAM_FORWARD " << D455_exist(msg) << std::endl;
    d455_server.sendMsg(ss.str());
}

void init()
{
    d455_server.serverStart();
}

int main(int argc, char **argv)
{
    init();

    ros::init(argc, argv, "d455_handle");
    ros::NodeHandle nh;

    ros::Subscriber sub = nh.subscribe("/d400/color/image_raw", 10, D455Callback);

    ros::spin();

    /************************/

    d455_server.serverClose();

    return 0;
}