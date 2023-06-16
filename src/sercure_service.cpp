#include "/home/pino/pino_ws/papi/PAPI.h"

/**************************************************/

volatile bool g_stop = false;

std::vector<std::string> kill_list;
std::vector<std::string> temp_vector;

/**************************************************/

void signalHandler(int signal)
{
    std::cout << "Received signal " << signal << ", stopping sercure service..." << std::endl;
    g_stop = true;
}

void addToKillList(std::string _node)
{
    temp_vector = PAPI::system::getPIDList(_node);
    kill_list.reserve(kill_list.size() + temp_vector.size());
    kill_list.insert(kill_list.end(), temp_vector.begin(), temp_vector.end());
    temp_vector.clear();
}

void killProcess()
{
    addToKillList("rosmaster");
    addToKillList("mavros");
    addToKillList("px4");
    addToKillList("gazebo");
    addToKillList("spinnaker");
    addToKillList("realsense2");
    addToKillList("peripherals_status");
    addToKillList("control_service");

    std::string command = "";
    for (auto it = kill_list.begin(); it != kill_list.end(); ++it)
    {
        command = "kill -9 " + *it;
        std::system(command.c_str());
        command.clear();

        std::cout << *it << " ";
    }
}

void driver_loader()
{
    std::string realsense_cmd = "roslaunch";
    std::vector<std::string> realsense_argv;
    realsense_argv.push_back("realsense2_camera");
    realsense_argv.push_back("rs_d400_and_t265.launch");
    realsense_argv.push_back("> /home/pino/logs/roslaunch_logs/realsense_log.log");
    realsense_argv.push_back("2>&1 &");

    std::string spinnaker_cmd = "roslaunch";
    std::vector<std::string> spinnaker_argv;
    spinnaker_argv.push_back("spinnaker_camera_driver");
    spinnaker_argv.push_back("color_cam.launch");
    spinnaker_argv.push_back("> /home/pino/logs/roslaunch_logs/spinnaker_log.log");
    spinnaker_argv.push_back("2>&1 &");

    PAPI::system::runCommand_system(realsense_cmd, realsense_argv);
    sleep(2); // Wait for performace

    PAPI::system::runCommand_system(spinnaker_cmd, spinnaker_argv);
    sleep(2); // Wait for performace
}

void start()
{
    driver_loader();
}

void close()
{
    killProcess();
}

void doThisInTheLoop()
{
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