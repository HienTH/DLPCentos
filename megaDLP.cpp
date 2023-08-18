#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <ctime>

void showHelp() {
    std::cout << "Usage: MegaDLP Agent For Centos [OPTIONS]" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  -h, --help         Show this help message" << std::endl;
    std::cout << "  -i                 Show Agent Infor " << std::endl;
    std::cout << "  --showlog          Show log information" << std::endl;
    std::cout << "  --enableusb        Enable USB Device Connection" << std::endl;
    std::cout << "  --disableusb       Disable USB Device Connection" << std::endl;
}

int agentinfor() {

    std::cout << "=== Mega DLP Agent For Centos ===" << std::endl;

    std::cout << "UserName: " << "dlp01" << std::endl;

    std::cout << "Email: " << "dlp01@gmail.com" << std::endl;

    std::cout << "Status: " << "Online" << std::endl;

    return 0;
}

void reloadUdevRules() {
    // Reload udev rules
    int reloadResult = std::system("sudo udevadm control --reload-rules");
    if (reloadResult != 0) {
        std::cerr << "Failed to reload udev rules." << std::endl;
        return;
    }

    // Trigger udev events
    int triggerResult = std::system("sudo udevadm trigger");
    if (triggerResult != 0) {
        std::cerr << "Failed to trigger udev events." << std::endl;
        return;
    }

    std::cout << "Udev rules applied successfully." << std::endl;

    return;
}

void createUdevRule(const std::string& ruleContent, const std::string& ruleFilename) {
    std::ofstream udevFile(("/etc/udev/rules.d/" + ruleFilename).c_str());
    if (!udevFile.is_open()) {
        std::cerr << "Failed to open udev rule file." << std::endl;
        return;
    }

    udevFile << ruleContent << std::endl;

    udevFile.close();
    std::cout << "Udev rule created: " << ruleFilename << std::endl;
    std::ofstream logFile("device_log.txt", std::ios::app);
    std::time_t currentTime = std::time(NULL);
    if (logFile.is_open()) {
        logFile << "Timestamp: " << std::ctime(&currentTime);
        logFile << "Udev rule created: " << ruleFilename << std::endl;
        logFile.close();
    } else {
        std::cerr << "Failed to open log file." << std::endl;
    }
}


void disableUSBDevice() {

    // Create a udev rule to disable USB devices
    std::string disableRule =
        "ACTION==\"add\", SUBSYSTEM==\"usb\", ATTR{idVendor}==\"1234\", ATTR{idProduct}==\"5678\", ATTR{authorized}=\"0\"";
    createUdevRule(disableRule, "99-disable-usb.rules");
    reloadUdevRules();
}

void enableUSBDevice() {
    // Create a udev rule to enable USB devices
    std::string enableRule =
        "ACTION==\"add\", SUBSYSTEM==\"usb\", ATTR{idVendor}==\"1234\", ATTR{idProduct}==\"5678\", ATTR{authorized}=\"1\"";
    createUdevRule(enableRule, "99-enable-usb.rules");
    reloadUdevRules();
}

int main(int argc, char* argv[]) {

    // Parse command-line arguments
    for (int i = 1; i < argc; ++i) {
        if (std::strcmp(argv[i], "-h") == 0 || std::strcmp(argv[i], "--help") == 0) {
            showHelp();
            return 0;
        } else if (std::strcmp(argv[i], "--infor") == 0) {
            agentinfor();
        } else if (std::strcmp(argv[i], "--showinfor") == 0) {
            //showinfor();
        } else if (std::strcmp(argv[i], "--enableusb") == 0) {
            enableUSBDevice();
        } else if (std::strcmp(argv[i], "--disableusb") == 0) {
            disableUSBDevice();
        } else {
            std::cerr << "Unknown option: " << argv[i] << std::endl;
            return 1;
        }
    }

    return 0;
}
