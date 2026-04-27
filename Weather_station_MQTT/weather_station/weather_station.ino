//Libraries and headder files
#include <ArduinoJson.h>
#include "m_wifi.h"
#include "m_lightning.h"
#include "m_mqtt.h"
#include "m_sensors.h"

//Timing variables
unsigned long lastSensors = 0;

const unsigned long sensorsInterval = 10000;  //miliseconds

void setup() {

  //Initialize serial communication
  Serial.begin(115200);

  //Connect to Wi-Fi network
  wifi_connection();

  //Initialize MQTT connection
  mqtt_init();

  //Initialize sensors
  sensors_init();

  //Initialize lightning detection
  lightning_init();
}

void loop() {

  // Maintain MQTT connection and handle communication
  mqtt_loop();

  // Get current time
  unsigned long now = millis();

  if (lightning_available()) {

    StaticJsonDocument<256> lightningDoc;

    // Read lightning data into JSON document
    lightning_read(lightningDoc);

    // Send data if valid
    if (!lightningDoc.isNull()) {
      mqtt_send(lightningDoc);
    }
  }

  if (now - lastSensors >= sensorsInterval) {
    lastSensors = now;

    StaticJsonDocument<768> doc;
    // Read sensor data into JSON document
    sensors_read(doc);

    // Send data only if MQTT is connected
    if (client.connected()) {
      mqtt_send(doc);
    }
  }
}