#include "peripherals_handle/peripherals_handle_lib.h"
#include "/home/pino/pino_ws/papi/PAPI.h"

PAPI::communication::Server server(DEFAULT_CAM_DOWNWARD_NODE_PORT);

void FLIRCallback(const wfov_camera_msgs::WFOVImage::ConstPtr &msg)
{
    std::cout << "FLIR Camera status: " << FLIR_exist(msg) << std::endl;

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