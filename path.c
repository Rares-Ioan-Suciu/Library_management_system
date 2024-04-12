#include "path.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#elif __linux__
#include <unistd.h>
#endif

char* getExecutableDirectory() { // function that gets the directory where the project is stores in order to work with other files, on other computer

    // UNTESTED

    char* buffer = NULL;
#ifdef _WIN32  // if working on windows
    DWORD size = GetModuleFileName(NULL, NULL, 0);
    if (size != 0) {
        buffer = (char*)malloc(size);
        if (buffer != NULL) {
            if (GetModuleFileName(NULL, buffer, size) == 0) {
                free(buffer);
                buffer = NULL;
            } else {
                char* lastSlash = strrchr(buffer, '\\');
                if (lastSlash != NULL) {
                    *(lastSlash + 1) = '\0'; // Null-terminate after the last slash
                }
            }
        }
    }
#elif __linux__ // for linux

    char tempBuf[256]; // Temporary buffer to hold the path
    ssize_t size = readlink("/proc/self/exe", tempBuf, sizeof(tempBuf));
    if (size != -1) {
        buffer = (char*)malloc(size + 1); // Extra byte for null terminator
        if (buffer != NULL) {
            if (readlink("/proc/self/exe", buffer, size) != -1) {
                buffer[size] = '\0';
                char* lastSlash = strrchr(buffer, '/');
                if (lastSlash != NULL) {
                    *(lastSlash + 1) = '\0';
                }
            } else {
                free(buffer);
                buffer = NULL;
            }
        }
    }
#endif
    return buffer;
}
