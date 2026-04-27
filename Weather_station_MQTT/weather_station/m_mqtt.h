#pragma once

//JSON, MQTT, Wi-Fi libraries
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <WiFi.h>

// MQTT handling functions declaration
bool mqtt_reconnect();
void mqtt_init();
void mqtt_loop();
void mqtt_send(JsonDocument& doc);

//Wi-Fi client object
extern WiFiClient espClient;
//MQTT client object
extern PubSubClient client;