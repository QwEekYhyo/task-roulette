#include "MQTTClient.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ADDRESS "tcp://localhost:1883"
#define CLIENTID "TaskRouletteClient"
#define TOPIC "game/request"
#define QOS 1
#define TIMEOUT 10000L

int message_arrived(void* context, char* topicName, int topicLen, MQTTClient_message* message) {
    printf("Message arrived\n");
    printf("     topic: %s\n", topicName);
    printf("   message: ");

    char* payloadptr = message->payload;
    for (int i = 0; i < message->payloadlen; i++) {
        putchar(*payloadptr++);
    }
    putchar('\n');
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
    MQTTClient_deliveryToken token;
    MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;
    MQTTClient_setCallbacks(client, NULL, connection_lost, message_arrived, NULL);

    int rc;
    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        printf("Failed to connect, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }

    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    static const uint8_t PAYLOAD_LENGTH = 10;
    unsigned char payload[10];
    payload[0] = 1;
    pubmsg.payload = payload;
    pubmsg.payloadlen = PAYLOAD_LENGTH;
    pubmsg.qos = QOS;
    pubmsg.retained = 0;
    MQTTClient_publishMessage(client, TOPIC, &pubmsg, &token);
    printf("Waiting for publication of %s\n"
           "on topic %s for client with ClientID: %s\n",
           payload, TOPIC, CLIENTID);

    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
    return rc;
}
