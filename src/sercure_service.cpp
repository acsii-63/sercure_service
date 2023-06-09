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
    addToKillList("driver_loader");
    addToKillList("peripherals_status");
    addToKillList("control_service");
    addToKillList("control_pkg");
    addToKillList("\"nc -l -k -p\"");
    addToKillList("\"nc -l -p\"");
    addToKillList("calib");

    std::string command = "";
    for (auto it = kill_list.begin(); it != kill_list.end(); ++it)
    {
        command = "kill -9 " + *it;
        std::system(command.c_str());
        command.clear();
        PAPI::system::sleepLessThanASecond(0.1);
        std::cout << *it << " ";
    }
}

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
    // message_listener();
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

void clearDir(const std::string &_dir)
{
    std::string cmd = "rm";
    std::vector<std::string> argv;
    argv.push_back("-rf");
    argv.push_back(_dir);
    argv.push_back("&&");
    argv.push_back("mkdir");
    argv.push_back("-p");
    argv.push_back(_dir);

    PAPI::system::runCommand_system(cmd, argv);
}

void close()
{
    killProcess();
    // clearMessageFile();

    clearDir(DEFAULT_MISSION_DIR_PATH);
    PAPI::system::sleepLessThanASecond(0.1);
    clearDir(DEFAULT_IMAGE_DIR_PATH);
    PAPI::system::sleepLessThanASecond(0.1);

    std::string cmd = "touch";
    std::vector<std::string> argv;
    argv.push_back(DEFAULT_MESSAGE_FILE_PATH);
    PAPI::system::runCommand_system(cmd, argv);
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