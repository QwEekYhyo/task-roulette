#ifndef TROULETTE_UTILS_H
#define TROULETTE_UTILS_H

#include <stdint.h>
#include <time.h>

static const uint8_t UUID_STR_LEN = 24;

extern const char HEX_CHARS[];

void generate_uuid(char buffer[UUID_STR_LEN]);

pid_t get_nth_process(unsigned int n);

#endif
