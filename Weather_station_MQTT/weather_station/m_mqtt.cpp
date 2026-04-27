#define MQTT_BUFFER_SIZE 768  // Size of MQTT message buffer
#include <ArduinoJson.h>      // JSON handling
#include <PubSubClient.h>     // MQTT client library
#include <WiFi.h>             // Wi-Fi functionality
#include "secret.h"           // Contains MQTT credentials (server, port, topic)
#include "m_mqtt.h"


WiFiClient espClient;
PubSubClient client(espClient);

static unsigned long lastReconnectAttempt = 0;

// Attempts to reconnect to MQTT broker
bool mqtt_reconnect() {
  if (client.connected()) return true;

  // Create unique client ID using MAC address
  String clientId = "ESP23Client-";
  clientId += WiFi.macAddress();
  // Try to connect to MQTT broker
  if (client.connect(clientId.c_str())) {
    Serial.println("MQTT connected");
    return true;
  } else {
    // Print error code if connection fails
    Serial.print("MQTT failed, rc=");
    Serial.println(client.state());
    return false;
  }
}

// Initialize MQTT client settings
void mqtt_init() {
  client.setServer(MQTT_SERVER, MQTT_PORT);
  client.setBufferSize(MQTT_BUFFER_SIZE);
}

void mqtt_loop() {
  // If not connected, try reconnecting every 5 seconds
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

// Send JSON data via MQTT
void mqtt_send(JsonDocument& doc) {
  // Convert JSON document to char array
  char payload[MQTT_BUFFER_SIZE];
  serializeJson(doc, payload);

  // Print payload for debugging
  Serial.print("MQTT payload: ");
  Serial.println(payload);

  // Publish message to MQTT topic
  if (client.publish(MQTT_TOPIC, payload)) {
    Serial.println("Publish OK");
  } else {
    Serial.println("Publish FAILED");
  }
}
