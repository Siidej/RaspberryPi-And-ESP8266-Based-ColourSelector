#include "MQTT_Subscriber.h"
#include "static/Servo.h"

#include <iostream>

void on_connect(struct mosquitto *mosq, void *obj, int result)
{
    if(!result){
        /* Subscribe to broker information topics on successful connect. */
        mosquitto_subscribe(mosq, NULL, "Trier", QOS);
    }else{
        printf("Connect failed\n");
    }
}

void on_message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *message)
{
    if(message->payloadlen){
        
        char *colour =(char*)message->payload;
        Servo::setPosition(colour);
        std::cout << colour << std::endl;
        //printf("%s\n", (char*)message->payload);
    }else{
        printf("%s (null)\n", message->topic);
    }
    fflush(stdout);
}

int main(int argc, char *argv[])
{

    struct mosquitto *mosq = NULL;
    mosquitto_lib_init();

    mosq = mosquitto_new(NULL, true, NULL);
    if(!mosq){
        printf("create client failed..\n");
        mosquitto_lib_cleanup();
        return 1;
    }
    
    mosquitto_connect_callback_set(mosq, on_connect);
    mosquitto_message_callback_set(mosq, on_message);

    if(mosquitto_connect(mosq, HOST, PORT, KEEP_ALIVE)){
        mosquitto_connect(mosq, HOST, PORT, KEEP_ALIVE);
        printf("Unable to connect.\n");
        return 1;
        }
    mosquitto_subscribe(mosq, NULL, TOPIC, QOS);
    mosquitto_loop_start(mosq);
    mosquitto_loop_forever(mosq, -1, 1);

    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();
    return 0;
}
