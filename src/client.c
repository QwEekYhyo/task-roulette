#include <signal.h>
#include <utils.h>

#include "MQTTClient.h"
#include <time.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BROKER_ADDRESS "tcp://localhost:1883"
#define TOPIC "GameEvents"
#define QOS 1
#define TIMEOUT 10000L

MQTTClient client;
MQTTClient_message pubmsg = MQTTClient_message_initializer;
const uint8_t PAYLOAD_LENGTH = 101;
char payload[] = "-QwEekYhyo";

int message_arrived(void* context, char* topicName, int topicLen, MQTTClient_message* message) {
    char* payloadptr = message->payload;

    if (*payloadptr == 100) {
        printf("Server is going to kill one process...\n");
        printf("Prepare to DIE!\n");

        pid_t pid = (uint8_t) *(payloadptr + 1);
        printf("Process %d is about to get killed\n", pid);
        if (kill(pid, SIGINT) == 0) {
            printf("SIGINT sent to process %d successfully.\n", pid);
        } else {
            perror("Failed to send SIGINT");
        }

        payload[0] = 25;
        pubmsg.payload = payload;
        MQTTClient_publishMessage(client, TOPIC, &pubmsg, NULL);
    } else if (*payloadptr == 105)
        printf("Player %s died.\n", payloadptr + 1);

    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);

    return 1;
}

void connection_lost(void* context, char* cause) {
    printf("\nConnection lost\n");
    printf("     cause: %s\n", cause);
}

int main(int argc, char* argv[]) {
    srand(time(NULL));

    char client_id[UUID_STR_LEN];
    generate_uuid(client_id);

    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    MQTTClient_create(&client, BROKER_ADDRESS, client_id, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;
    MQTTClient_setCallbacks(client, NULL, connection_lost, message_arrived, NULL);

    int rc;
    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        printf("Failed to connect, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }

    payload[0] = 1;
    pubmsg.payload = payload;
    pubmsg.payloadlen = PAYLOAD_LENGTH;
    pubmsg.qos = QOS;
    pubmsg.retained = 0;
    printf("Joining game...\n");
    MQTTClient_publishMessage(client, TOPIC, &pubmsg, NULL);
    MQTTClient_subscribe(client, TOPIC, QOS);
    printf("Joined game!\n");

    payload[0] = 5;
    pubmsg.payload = payload;
    MQTTClient_publishMessage(client, TOPIC, &pubmsg, NULL);

    int ch;
    while (1) {
        ch = getchar();
        if (ch == 'Q' || ch == 'q') break;
    }

    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
    return rc;
}
