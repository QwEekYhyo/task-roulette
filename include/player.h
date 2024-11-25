#ifndef TROULETTE_PLAYER_H
#define TROULETTE_PLAYER_H

#include <stdint.h>

#define MAX_NAME_LEN 100

typedef struct Player {
    char name[MAX_NAME_LEN];
    uint8_t is_alive;
    struct Player* next;
} Player;

Player* create_player(const char* name, int is_alive);

void add_player(Player** head, const char* name);

Player* find_player(Player* head, const char* name);

uint8_t everyone_survived(Player* head);

void kill_players(Player* head);

void free_players(Player* head);

void display_players(Player* head);

#endif
