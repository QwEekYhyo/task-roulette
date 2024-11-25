#ifndef TROULETTE_UTILS_H
#define TROULETTE_UTILS_H

#define UUID_STR_LEN 24

extern const char HEX_CHARS[];

void generate_uuid(char buffer[UUID_STR_LEN]);

#endif
