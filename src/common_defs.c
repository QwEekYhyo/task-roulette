#include <common_defs.h>

#include <stdio.h>

void connection_lost(void* context, char* cause) {
    printf("\nConnection lost\n");
    printf("     cause: %s\n", cause);
}
