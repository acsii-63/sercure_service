#include "../papi/PAPI.h"

#include <csignal>

volatile bool g_stop = false;

void signalHandler(int signal)
{
    std::cout << "Received signal " << signal << ", stopping sercure service..." << std::endl;
    g_stop = true;
}

void doThisInTheLoop()
{
    return;
}

int main()
{
    /*    Run until the sercure service get Ctrl+C    */

    // Register signal handler for SIGINT (Ctrl+C)
    std::signal(SIGINT, signalHandler);

    while (!g_stop)
    {
        doThisInTheLoop();
    }

    /***************************************************/

    /*                    Run once                     */

    std::vector<std::string> kill_list;
    std::vector<std::string> temp_vector;
    std::string command = "";

    temp_vector = PAPI::system::getPIDList("rosmaster");
    kill_list.reserve(kill_list.size() + temp_vector.size());
    kill_list.insert(kill_list.end(), temp_vector.begin(), temp_vector.end());
    temp_vector.clear();

    temp_vector = PAPI::system::getPIDList("mavros");
    kill_list.reserve(kill_list.size() + temp_vector.size());
    kill_list.insert(kill_list.end(), temp_vector.begin(), temp_vector.end());
    temp_vector.clear();

    temp_vector = PAPI::system::getPIDList("px4");
    kill_list.reserve(kill_list.size() + temp_vector.size());
    kill_list.insert(kill_list.end(), temp_vector.begin(), temp_vector.end());
    temp_vector.clear();

    temp_vector = PAPI::system::getPIDList("gazebo");
    kill_list.reserve(kill_list.size() + temp_vector.size());
    kill_list.insert(kill_list.end(), temp_vector.begin(), temp_vector.end());
    temp_vector.clear();

    for (auto it = kill_list.begin(); it != kill_list.end(); ++it)
    {
        command = "kill -9 " + *it;
        std::system(command.c_str());
        command.clear();

        std::cout << *it << " ";
    }

    /***************************************************/

    std::cout << "Sercure service  stopped." << std::endl;
    return 0;
}