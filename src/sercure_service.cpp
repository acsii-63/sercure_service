#include "/home/pino/pino_ws/papi/PAPI.h"

/**************************************************/

volatile bool g_stop = false;

std::vector<std::string> kill_list;
std::vector<std::string> temp_vector;

std::vector<int> peripheral_list;

// PAPI::communication::Client client_CAM_FORWARD(LOCAL_HOST, DEFAULT_CAM_FORWARD_NODE_PORT);
// PAPI::communication::Client client_CAM_ODOM(LOCAL_HOST, DEFAULT_CAM_ODOM_NODE_PORT);
// PAPI::communication::Client client_CAM_DOWNWARD(LOCAL_HOST, DEFAULT_CAM_DOWNWARD_NODE_PORT);
// PAPI::communication::Client client_LIDAR(LOCAL_HOST, DEFAULT_LIDAR_NODE_PORT);
// PAPI::communication::Client client_FCU(LOCAL_HOST, DEFAULT_FCU_NODE_PORT);

/**************************************************/

void signalHandler(int signal)
{
    std::cout << "Received signal " << signal << ", stopping sercure service..." << std::endl;
    g_stop = true;
}

void addNodeToKillList(std::string _node)
{
    temp_vector = PAPI::system::getPIDList(_node);
    kill_list.reserve(kill_list.size() + temp_vector.size());
    kill_list.insert(kill_list.end(), temp_vector.begin(), temp_vector.end());
    temp_vector.clear();
}

void killAllNode()
{
    addNodeToKillList("rosmaster");
    addNodeToKillList("mavros");
    addNodeToKillList("px4");
    addNodeToKillList("gazebo");
    // addNodeToKillList("flir_handle");
    // addNodeToKillList("d455_handle");
    // addNodeToKillList("t265_handle");
    addNodeToKillList("spinnaker");
    addNodeToKillList("realsense2");
    addNodeToKillList("peripherals");
    addNodeToKillList("control_service");

    std::string command = "";
    for (auto it = kill_list.begin(); it != kill_list.end(); ++it)
    {
        command = "kill -9 " + *it;
        std::system(command.c_str());
        command.clear();

        std::cout << *it << " ";
    }
}

// void addPeripheralsList()
// {
//     peripheral_list.push_back(Peripheral::PERIPHERAL_CAM_DOWNWARD);
//     peripheral_list.push_back(Peripheral::PERIPHERAL_CAM_FORWARD);
//     peripheral_list.push_back(Peripheral::PERIPHERAL_CAM_ODOM);
// }

// int startClient(const int _index)
// {
//     switch (_index)
//     {
//     case Peripheral::PERIPHERAL_CAM_DOWNWARD:
//         return client_CAM_DOWNWARD.clientStart();
//         break;

//     case Peripheral::PERIPHERAL_CAM_FORWARD:
//         return client_CAM_FORWARD.clientStart();
//         break;

//     case Peripheral::PERIPHERAL_CAM_ODOM:
//         return client_CAM_ODOM.clientStart();
//         break;

//     case Peripheral::PERIPHERAL_LIDAR:
//         return client_LIDAR.clientStart();
//         break;

//     case Peripheral::PERIPHERAL_PCU:
//         return client_FCU.clientStart();
//         break;

//     default:
//         std::cerr << "Invalid Peripheral index.\n";
//         return 1;
//         break;
//     }
// }

// void startPeripheralsClient(const std::vector<int> &_peripherals)
// {
//     std::vector<int> peripherals = _peripherals;
//     for (auto peripheral_index = 0; peripheral_index < peripherals.size(); peripheral_index++)
//     {
//         int current_result = startClient(peripherals[peripheral_index]);
//         if (current_result == -1)
//             std::cerr << "Cannot connect with no." << peripheral_index << " peripheral Server.\n";
//     }
// }

// void closeClient(const int _index)
// {
//     switch (_index)
//     {
//     case Peripheral::PERIPHERAL_CAM_DOWNWARD:
//         client_CAM_DOWNWARD.clientClose();
//         break;

//     case Peripheral::PERIPHERAL_CAM_FORWARD:
//         client_CAM_FORWARD.clientClose();
//         break;

//     case Peripheral::PERIPHERAL_CAM_ODOM:
//         client_CAM_ODOM.clientClose();
//         break;

//     case Peripheral::PERIPHERAL_LIDAR:
//         client_LIDAR.clientClose();
//         break;

//     case Peripheral::PERIPHERAL_PCU:
//         client_FCU.clientClose();
//         break;
//     }
// }

// void closePeripheralsClient(const std::vector<int> &_peripherals)
// {
//     std::vector<int> peripherals = _peripherals;
//     for (auto peripheral_index = 0; peripheral_index < peripherals.size(); peripheral_index++)
//     {
//         closeClient(peripherals[peripheral_index]);
//     }
// }

void start()
{
    // addPeripheralsList();
    // startPeripheralsClient(peripheral_list);
    std::string control_srv_cmd = "cd";
    std::vector<std::string> control_srv_argv;
    control_srv_argv.push_back("/home/pino/pino_ws/control_service/src");
    control_srv_argv.push_back("&&");
    control_srv_argv.push_back("./control_service");

    std::string peripheral_cmd = "rosrun";
    std::vector<std::string> peripheral_argv;
    peripheral_argv.push_back("peripherals_status");
    peripheral_argv.push_back("automatic");

    PAPI::system::runCommand_system(control_srv_cmd, control_srv_argv);
    sleep(5); // Wait for performace

    PAPI::system::runCommand_system(peripheral_cmd, peripheral_argv);
}

void close()
{
    // closePeripheralsClient(peripheral_list);
    killAllNode();
}

void doThisInTheLoop()
{
    // std::cout << client_CAM_DOWNWARD.reciveMessage() << " | "
    //           << client_CAM_FORWARD.reciveMessage() << " | "
    //           << client_CAM_ODOM.reciveMessage() << std::endl;

    return;
}

int main()
{
    /*                     Run once                    */

    // Register signal handler for SIGINT (Ctrl+C)
    std::signal(SIGINT, signalHandler);

    start();

    /***************************************************/

    while (!g_stop)
    {
        /* Run until the sercure service get Ctrl+C */
        doThisInTheLoop();
    }

    /***************************************************/
    /*               Run after get Ctrl+C              */

    close();

    std::cout << "Sercure service stopped." << std::endl;
    return 0;
}