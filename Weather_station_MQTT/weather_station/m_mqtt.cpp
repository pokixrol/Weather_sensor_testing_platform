#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include "secret.h"
#include "m_wifi.h"
#include "m_mqtt.h"


WiFiClient espClient;
PubSubClient client(espClient);

static unsigned long lastReconnectAttempt = 0;

bool mqtt_reconnect() {
  if (client.connected()) return true;

  String clientId = "ESP23Client-";
  clientId += WiFi.macAddress();

  if (client.connect(clientId.c_str())) {
    Serial.println("MQTT connected");
    return true;
  } else {
    Serial.print("MQTT failed, rc=");
    Serial.println(client.state());
    return false;
  }
}

void mqtt_init() {
  client.setServer(MQTT_SERVER, MQTT_PORT);
}

void mqtt_loop() {
  if (!client.connected()) {
    unsigned long now = millis();
    if (now - lastReconnectAttempt > 5000) {
      lastReconnectAttempt = now;
      mqtt_reconnect();
    }
  } else {
    client.loop();
  }
}

void mqtt_send(float t) {
  StaticJsonDocument<128> doc;
  char payload[128];

  doc["t"] = t;
  
  serializeJson(doc, payload);

  client.publish(MQTT_TOPIC, payload);
}