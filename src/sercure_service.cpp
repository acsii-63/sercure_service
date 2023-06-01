#include "/home/pino/pino_ws/papi/PAPI.h"

std::vector<std::string> kill_list;
std::vector<std::string> temp_vector;

volatile bool g_stop = false;

void signalHandler(int signal)
{
    std::cout << "Received signal " << signal << ", stopping sercure service..." << std::endl;
    g_stop = true;
}

PAPI::communication::Client handlePeripherals("127.0.0.1", DEFAULT_CAM_FORWARD_NODE_PORT);

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
    addNodeToKillList("flir_handle");
    addNodeToKillList("d455_handle");
    addNodeToKillList("t265_handle");

    std::string command = "";
    for (auto it = kill_list.begin(); it != kill_list.end(); ++it)
    {
        command = "kill -9 " + *it;
        std::system(command.c_str());
        command.clear();

        std::cout << *it << " ";
    }
}

void doThisInTheLoop()
{
    std::cout << handlePeripherals.reciveMessage();

    return;
}

int main()
{
    /*                     Run once                    */

    // Register signal handler for SIGINT (Ctrl+C)
    std::signal(SIGINT, signalHandler);
    handlePeripherals.clientStart();
    std::cout << handlePeripherals.reciveMessage();

    /***************************************************/

    while (!g_stop)
    {
        /* Run until the sercure service get Ctrl+C */
        doThisInTheLoop();
    }

    /***************************************************/
    /*               Run after get Ctrl+C              */

    handlePeripherals.clientClose();

    killAllNode();

    std::cout << "Sercure service stopped." << std::endl;
    return 0;
}