#include <common_defs.h>
#include <server_utils.h>

#include <string.h>

void player_died_alert(MQTTClient* client, const char* player_name) {
    char payload[PLAYER_DEATH_PAYLOAD_LENGTH];

    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    strncpy(payload + 1, player_name, PLAYER_DEATH_PAYLOAD_LENGTH - 1);
    payload[0] = 105; // Event code for player death
    pubmsg.payload = payload;
    pubmsg.payloadlen = PLAYER_DEATH_PAYLOAD_LENGTH;
    MQTTClient_publishMessage(*client, TOPIC, &pubmsg, NULL);
}
