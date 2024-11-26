#include <utils.h>

#include <ctype.h>
#include <dirent.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

const char HEX_CHARS[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

void generate_uuid(char buffer[UUID_STR_LEN]) {
    for(uint8_t i = 0; i < UUID_STR_LEN; i++) {
        buffer[i] = HEX_CHARS[rand() % 16];
    }
}

pid_t get_nth_process(unsigned int n) {
    DIR *proc_directory = opendir("/proc");
    if (!proc_directory) {
        perror("opendir");
        return -1;
    }

    unsigned int count = 0;
    struct dirent* result;
    struct dirent* entry;
    while ((entry = readdir(proc_directory)) != NULL && count < n) {
        if (isdigit(entry->d_name[0])) {
            result = entry;
            count++;
        }
    }

    if (!result) return -1;
    return atoi(result->d_name);
}
