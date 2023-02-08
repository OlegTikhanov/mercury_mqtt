#ifndef MQTT_H
#define MQTT_H
#include "settings.h"



void mqtt_init(void);
int mqtt_send(char *msg, char *topic);
#endif
