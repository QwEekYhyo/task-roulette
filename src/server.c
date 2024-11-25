#include <player.h>

#include "MQTTClient.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <threads.h>

#define BROKER_ADDRESS "tcp://localhost:1883"
#define CLIENTID "TaskRouletteServer"
#define TOPIC "GameEvents"
#define QOS 1
#define TIMEOUT 10000L

// I don't know what will happen when multiple threads are going to try to add players to this list
Player* players = NULL;
volatile uint8_t is_turn_playing = 0;
thrd_t death_checker_thread;

int check_deaths(void* arg) {
    thrd_sleep(&(struct timespec){.tv_sec=3}, NULL);
    if (!is_turn_playing) return 1;

    Player* current_player = (Player*) arg;
    while (current_player != NULL) {
        if (current_player->is_alive == 0) {
            // publish something in GameEvents
            printf("%s died.\n", current_player->name);
            current_player->is_alive = 2;
        }
        current_player = current_player->next;
    }
    return 0;
}

int message_arrived(void* context, char* topicName, int topicLen, MQTTClient_message* message) {
    char* payloadptr = message->payload;
    switch (*payloadptr) {
        case 1:
            printf("Someone asked to join the game\n");
            add_player(&players, ++payloadptr);
            printf("New player list:\n");
            display_players(players);
            putchar('\n');
            break;
        case 5:
            if (is_turn_playing) return 1;
            is_turn_playing = 1;
            printf("Someone asked to play a turn\n");
            kill_players(players);

            thrd_create(&death_checker_thread, check_deaths, players);
            break;
        case 25:
            if (!is_turn_playing) return 1;
            printf("Someone updated its status\n");

            Player* player = find_player(players, ++payloadptr);
            if (!player) return 1;
            player->is_alive = 1;

            if (everyone_survived(players)) {
                is_turn_playing = 0;
                printf("Everyone survived this turn!\n");
            }
            break;
    }

    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}

void connection_lost(void* context, char* cause) {
    printf("\nConnection lost\n");
    printf("     cause: %s\n", cause);
}

int main(int argc, char* argv[]) {
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    int rc;
    MQTTClient_create(&client, BROKER_ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;

    MQTTClient_setCallbacks(client, NULL, connection_lost, message_arrived, NULL);
    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        printf("Failed to connect, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }
    printf("Subscribing to topic %s\nfor client %s using QoS%d\n\n"
           "Press Q<Enter> to quit\n\n",
           TOPIC, CLIENTID, QOS);
    MQTTClient_subscribe(client, TOPIC, QOS);

    int ch;
    while (1) {
        ch = getchar();
        if (ch == 'Q' || ch == 'q') break;
    }

    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);

    free_players(players);

    return rc;
}
