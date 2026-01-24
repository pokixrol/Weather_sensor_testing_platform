#pragma once

#include <WiFi.h>
#include <PubSubClient.h>

bool mqtt_reconnect();
void mqtt_init();
void mqtt_loop();
void mqtt_send(float);

extern WiFiClient espClient;
extern PubSubClient client;