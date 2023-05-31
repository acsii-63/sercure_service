#include "../papi/PAPI.h"

#include <csignal>

volatile bool g_stop = false;

// Clear screen and move cursor to the top-left corner
void clearScreen()
{
    std::cout << "\033[2J";   // Clear screen
    std::cout << "\033[0;0f"; // Move cursor to the top-left corner
}

void signalHandler(int signal)
{
    std::cout << "Received signal " << signal << ", stopping..." << std::endl;
    g_stop = true;
}

/*
WAITING_FOR_HOME_POSE = 0
TAKE_OFF = 1
HOLD = 2
MISSION_EXECUTION = 3
AUTO_LAND = 4
ONGROUND = 5
*/
std::string STATE_enumToString(int _enum)
{
    switch (_enum)
    {
    case UAV_STATE::WAITING_FOR_HOME_POSE:
        return "WAITING_FOR_HOME_POSE";
        break;

    case UAV_STATE::TAKE_OFF:
        return "TAKE_OFF";
        break;

    case UAV_STATE::HOLD:
        return "HOLD";
        break;

    case UAV_STATE::MISSION_EXECUTION:
        return "MISSION_EXECUTION";
        break;

    case UAV_STATE::AUTO_LAND:
        return "AUTO_LAND";
        break;

    case UAV_STATE::ONGROUND:
        return "ONGROUND";
        break;

    default:
        return "UNSPECIFIED STATE";
        break;
    }
}

int main()
{
    // Register signal handler for SIGINT (Ctrl+C)
    std::signal(SIGINT, signalHandler);

    while (!g_stop)
    {
        clearScreen();
        int current_state = PAPI::drone::getState();
        std::cout << "STATE: " << current_state << " | " << STATE_enumToString(current_state) << std::endl;
        
        // Wait for a while (for demonstration purposes)
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}
