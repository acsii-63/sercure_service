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
    addToKillList("\"nc -l -k -p\"");

    std::string command = "";
    for (auto it = kill_list.begin(); it != kill_list.end(); ++it)
    {
        command = "kill -9 " + *it;
        std::system(command.c_str());
        command.clear();

        std::cout << *it << " ";
    }
}

// void driver_loader()
// {
//     std::string realsense_cmd = "roslaunch";
//     std::vector<std::string> realsense_argv;
//     realsense_argv.push_back("realsense2_camera");
//     realsense_argv.push_back("rs_d400_and_t265.launch");
//     realsense_argv.push_back("> /home/pino/logs/roslaunch_logs/realsense_log.log");
//     realsense_argv.push_back("2>&1 &");

//     std::string spinnaker_cmd = "roslaunch";
//     std::vector<std::string> spinnaker_argv;
//     spinnaker_argv.push_back("spinnaker_camera_driver");
//     spinnaker_argv.push_back("color_cam.launch");
//     spinnaker_argv.push_back("> /home/pino/logs/roslaunch_logs/spinnaker_log.log");
//     spinnaker_argv.push_back("2>&1 &");

//     PAPI::system::runCommand_system(realsense_cmd, realsense_argv);
//     sleep(2); // Wait for performace

//     PAPI::system::runCommand_system(spinnaker_cmd, spinnaker_argv);
//     sleep(2); // Wait for performace
// }

void message_listener()
{
    std::string cmd = "nc";

    std::vector<std::string> argv;
    argv.push_back("-l -k -p");
    argv.push_back(std::to_string(DEFAULT_CONTROL_CONFIRM_PORT));
    argv.push_back(">>");
    argv.push_back(DEFAULT_MESSAGE_FILE_PATH);
    argv.push_back("&");

    PAPI::system::runCommand_system(cmd, argv);
}

void start()
{
    // driver_loader();
    message_listener();
}

void clearMessageFile()
{
    std::string cmd;
    std::vector<std::string> argv;

    cmd = "rm";
    argv.push_back("-f");
    argv.push_back(DEFAULT_MESSAGE_FILE_PATH);
    PAPI::system::runCommand_system(cmd, argv);
    PAPI::system::sleepLessThanASecond(0.1);
    cmd.clear();
    argv.clear();

    cmd = "touch";
    argv.push_back(DEFAULT_MESSAGE_FILE_PATH);
    PAPI::system::runCommand_system(cmd, argv);
}

void close()
{
    killProcess();
    clearMessageFile();
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

    std::cout << "\nSercure service stopped." << std::endl;
    return 0;
}