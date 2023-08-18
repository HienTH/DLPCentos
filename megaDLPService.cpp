#include <iostream>
#include <cstring>
#include <fstream>
#include <unistd.h>
#include <sys/inotify.h>
#include <ftw.h>
#include <ctime>

void handleEvent(std::ofstream& outFile, const char* eventType, const char* fileName) {
    std::time_t currentTime = std::time(NULL);
    char timeBuffer[80];
    std::strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d %H:%M:%S", std::localtime(&currentTime));

    outFile << "Time: " << timeBuffer << " - Event: " << eventType << " - File: " << fileName << std::endl;

}


int handleFile(const char* filePath, const struct stat* fileStatus, int typeFlag, struct FTW* ftwbuf) {
    if (typeFlag == FTW_F) {
        // Ignore regular files, only monitor directories
        return 0;
    }

    int fd = inotify_init();
    if (fd == -1) {
        std::cerr << "Failed to initialize inotify." << std::endl;
        return 1;
    }

    int wd = inotify_add_watch(fd, filePath, IN_CREATE | IN_MODIFY | IN_DELETE);
    if (wd == -1) {
        std::cerr << "Failed to add watch to directory." << std::endl;
        close(fd);
        return 1;
    }

    std::ofstream outFile("file_transfer_log.txt", std::ios::app);
    if (!outFile.is_open()) {
        std::cerr << "Failed to open log file." << std::endl;
        inotify_rm_watch(fd, wd);
        close(fd);
        return 1;
    }
    char buffer[4096];
    while (true) {
        int length = read(fd, buffer, sizeof(buffer));
        if (length == -1) {
            std::cerr << "Error while reading events." << std::endl;
            break;
        }

        for (char* ptr = buffer; ptr < buffer + length;) {
            struct inotify_event* event = reinterpret_cast<struct inotify_event*>(ptr);
            if (event->len > 0) {
                const char* eventType;
                if (event->mask & IN_CREATE) {
                    eventType = "File created";
                } else if (event->mask & IN_MODIFY) {
                    eventType = "File modified";
                } else if (event->mask & IN_DELETE) {
                    eventType = "File deleted";
                } else {
                    eventType = "Unknown";
                }
                handleEvent(outFile, eventType, event->name);
            }
            ptr += sizeof(struct inotify_event) + event->len;
        }
    }

    inotify_rm_watch(fd, wd);
    close(fd);
    outFile.close();

    return 0;
}

int main() {
	
    const char* folderPath = "/media/"; // Replace with the folder path you want to monitor

    int flags = FTW_PHYS; // FTW_PHYS: avoid following symbolic links
    int maxfd = 10; // Maximum number of file descriptors to keep open simultaneously during tree walk

    int result = nftw(folderPath, handleFile, maxfd, flags);
    if (result == -1) {
        std::cerr << "Error during file tree walk." << std::endl;
        return 1;
    }

    return 0;
}

