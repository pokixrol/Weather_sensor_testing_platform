#include <SparkFun_AS3935.h>
#include <Adafruit_LTR390.h>
#include <Adafruit_SHT4x.h>
#include <Adafruit_BMP280.h>
#include "Adafruit_BME680.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_TSL2561_U.h>
#include <ArduinoJson.h>
#include <BH1750.h>
#include "m_wifi.h"
#include "m_lightning.h"
#include "m_mqtt.h"
#include "m_sensors.h"
#include "secret.h"
#include <Wire.h>

unsigned long last = 0;
const unsigned long interval = 2000;

void setup() {

  Serial.begin(115200);

  sensors_init();
  lightning_init();

  wifi_connection();
  mqtt_init();
}

void loop() {

  mqtt_loop();

  if (lightning_available()) {

    StaticJsonDocument<256> lightningDoc;
    lightning_read(lightningDoc);

    if (!lightningDoc.isNull()) {
      mqtt_send(lightningDoc);
    }
  }

  if (millis() - last >= interval) {
    last = millis();

    StaticJsonDocument<512> doc;
    sensors_read(doc);

    if (client.connected()) {
      mqtt_send(doc);
    }
  }
}