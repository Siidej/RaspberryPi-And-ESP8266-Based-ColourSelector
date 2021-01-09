#pragma once
#include <stdio.h>
#include <mosquitto.h>

#define HOST "localhost"
#define PORT  1883
#define KEEP_ALIVE 60
#define TOPIC "Trier"
#define QOS 1


void on_connect(struct mosquitto *mosq, void *obj, int result);
void on_message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *message);
