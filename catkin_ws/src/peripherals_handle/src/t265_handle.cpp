#include "peripherals_handle/peripherals_handle_lib.h"
#include "/home/pino/pino_ws/papi/PAPI.h"

PAPI::communication::Server server(DEFAULT_CAM_ODOM_NODE_PORT);

void odomCallback(const nav_msgs::Odometry::ConstPtr &msg)
{
    std::cout << "T265 Camera status: " << T265_exist(msg) << std::endl;

    std::stringstream ss;
    ss << "CAM_ODOM " << T265_exist(msg) << std::endl;
    server.sendMsg(ss.str());
}

void init()
{
    server.serverStart();
}

int main(int argc, char **argv)
{
    init();

    ros::init(argc, argv, "t265_handle");
    ros::NodeHandle nh;

    ros::Subscriber sub = nh.subscribe("/t265/odom/sample", 10, odomCallback);

    ros::spin();

    /************************/

    server.serverClose();

    return 0;
}