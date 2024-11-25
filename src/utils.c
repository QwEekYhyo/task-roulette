#include <utils.h>

#include <stdlib.h>
#include <stdint.h>

const char HEX_CHARS[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

void generate_uuid(char buffer[UUID_STR_LEN]) {
    for(uint8_t i = 0; i < UUID_STR_LEN; i++) {
        buffer[i] = HEX_CHARS[rand() % 16];
    }
}
