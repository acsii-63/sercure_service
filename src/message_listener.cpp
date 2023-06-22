#include "/home/pino/pino_ws/papi/PAPI.h"

int main()
{
    std::string cmd = "nc";

    std::vector<std::string> argv;
    argv.push_back("-l -k -p");
    argv.push_back(std::to_string(DEFAULT_CONTROL_CONFIRM_PORT));
    argv.push_back(">>");
    argv.push_back(DEFAULT_MESSAGE_FILE_PATH);
    argv.push_back("&");

    PAPI::system::runCommand_system(cmd, argv);

    return 0;
}