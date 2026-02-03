#pragma once

#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <WiFi.h>

bool mqtt_reconnect();
void mqtt_init();
void mqtt_loop();
void mqtt_send(JsonDocument& doc);

extern WiFiClient espClient;
extern PubSubClient client;