#ifndef TROULETTE_SERVER_UTILS_H
#define TROULETTE_SERVER_UTILS_H

#include "MQTTClient.h"
#include <stdint.h>

static const uint8_t PLAYER_DEATH_PAYLOAD_LENGTH = 101;

// Publish on topic that a player died
void player_died_alert(MQTTClient* client, const char* player_name);

#endif
