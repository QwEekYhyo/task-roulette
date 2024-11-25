#include "MQTTClient.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define BROKER_ADDRESS "tcp://localhost:1883"
#define CLIENTID "TaskRouletteServer"
#define TOPIC "GameEvents"
#define QOS 1
#define TIMEOUT 10000L

volatile Player* players = NULL;

int message_arrived(void* context, char* topicName, int topicLen, MQTTClient_message* message) {
    unsigned char* payloadptr = message->payload;
    if (*payloadptr == 1) {
        printf("Someone asked to join the game\n");
        
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

    return rc;
}
