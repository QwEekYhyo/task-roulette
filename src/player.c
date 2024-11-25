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

uint8_t everyone_survived(Player* head) {
    while (head != NULL) {
        if (head->is_alive == 0)
            return 0;
        head = head->next;
    }
    return 1;
}

void kill_players(Player* head) {
    while (head != NULL) {
        if (head->is_alive == 1)
            head->is_alive = 0;
        head = head->next;
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
        printf("Name: %s, Status: %s\n", head->name, head->is_alive == 1 ? "Alive" : "Dead");
        head = head->next;
    }
}
