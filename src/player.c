#include <player.h>

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

Player* create_player(const char* name, int is_alive) {
    Player* newPlayer = (Player*) malloc(sizeof(Player));
    strncpy(newPlayer->name, name, MAX_NAME_LEN);
    newPlayer->is_alive = is_alive;
    newPlayer->next = NULL;
    return newPlayer;
}

void add_player(Player** head, const char* name) {
    Player* newPlayer = create_player(name, 1);
    newPlayer->next = *head;
    *head = newPlayer;
}

Player* find_player(Player* head, const char* name) {
    while (head != NULL) {
        if (strcmp(head->name, name) == 0) {
            return head;
        }
        head = head->next;
    }
    return NULL;
}

void update_player_status(Player* head, const char* name, int new_status) {
    Player* player = find_player(head, name);
    if (player != NULL) {
        player->is_alive = new_status;
    } else {
        printf("Player %s not found.\n", name);
    }
}

void free_players(Player* head) {
    Player* tmp;
    while (head != NULL) {
        tmp = head;
        head = head->next;
        free(tmp);
    }
}

// This is only for debug purposes
void display_players(Player* head) {
    while (head != NULL) {
        printf("Name: %s, Status: %s\n", head->name, head->is_alive ? "Alive" : "Dead");
        head = head->next;
    }
}
