#include <common_defs.h>
#include <server_utils.h>

#include <stdlib.h>
#include <string.h>
#include <time.h>

void player_died_alert(MQTTClient* client, const char* player_name) {
    char payload[PLAYER_DEATH_PAYLOAD_LENGTH];

    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    strncpy(payload + 1, player_name, PLAYER_DEATH_PAYLOAD_LENGTH - 1);
    payload[0] = PLAYER_DIED_EVENT;
    pubmsg.payload = payload;
    pubmsg.payloadlen = PLAYER_DEATH_PAYLOAD_LENGTH;
    MQTTClient_publishMessage(*client, TOPIC, &pubmsg, NULL);
}

void no_player_died_alert(MQTTClient* client) {
    char payload[1];

    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    payload[0] = NO_PLAYER_DIED_EVENT;
    pubmsg.payload = payload;
    pubmsg.payloadlen = 1;
    MQTTClient_publishMessage(*client, TOPIC, &pubmsg, NULL);
}

void send_kill_instructions(MQTTClient* client) {
    srand(time(NULL));
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    char payload[2];
    payload[0] = KILL_PROCESS_EVENT;
    payload[1] = rand() % 256;
    pubmsg.payload = payload;
    pubmsg.payloadlen = 2;
    MQTTClient_publishMessage(*client, TOPIC, &pubmsg, NULL);
}
