#include <Adafruit_SHT4x.h>
#include <Adafruit_BMP280.h>
#include "Adafruit_BME680.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_TSL2561_U.h>
#include <ArduinoJson.h>
#include <BH1750.h>
#include "m_wifi.h"
#include "m_mqtt.h"
#include "secret.h"
#include <Wire.h>

Adafruit_BME680 bme688(&Wire);
BH1750 bh1750;
Adafruit_TSL2561_Unified tsl2561 = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 12345);
Adafruit_SHT4x sht45 = Adafruit_SHT4x();
Adafruit_SHT4x sht40 = Adafruit_SHT4x();
Adafruit_BMP280 bmp280(&Wire1);

void check_sensor(bool result, const char* name)
{
  if (!result) {
    Serial.println("%s not found !", name);
  }
}

void setup() {
  Serial.begin(115200);

  Wire.begin();
  Wire1.begin(19, 18);

  check_sensor(bme688.begin(0x76), "BME 688");
  check_sensor(sht45.begin(&Wire), "SHT 45");
  check_sensor(bh1750.begin(), "BH 1750");
  check_sensor(tsl2561.begin(), "TSL 2561");
  check_sensor(sht40.begin(&Wire1), "SHT 40");
  check_sensor(bmp280.begin(0x77), "BMP 280");

  wifi_connection();
  mqtt_init();
}

void loop() {

  mqtt_loop();

  StaticJsonDocument<1024> doc;

  sensors_event_t l, h1, t1, h2, t2;

  if (bme688.performReading()) {
    Serial.print("BME 688:\tTemperature = ");
    Serial.print(bme688.temperature);
    Serial.print(" °C\tHumidity = ");
    Serial.print(bme688.humidity);
    Serial.print(" %");
    Serial.println("");
    JsonObject bme = doc.createNestedObject("bme688");
    bme["temperature"] = bme688.temperature;
    bme["humidity"] = bme688.humidity;
    bme["pressure"] = bme688.pressure / 100.0;
  } else {
    Serial.println("BME 688 read error");
  }

  if (sht45.getEvent(&h1, &t1)) {
    Serial.print("SHT 45:\t\tTemperature = ");
    Serial.print(t1.temperature);
    Serial.print(" °C\tHumidity = ");
    Serial.print(h1.relative_humidity);
    Serial.print(" %");
    Serial.println("");
    JsonObject sht = doc.createNestedObject("sht45");
    sht["temperature"] = t1.temperature;
    sht["humidity"] = h1.relative_humidity;
  } else {
    Serial.println("SHT45 read error");
  }

  if (sht40.getEvent(&h2, &t2)) {
    Serial.print("SHT 40:\t\tTemperature = ");
    Serial.print(t2.temperature);
    Serial.print(" °C\tHumidity = ");
    Serial.print(h2.relative_humidity);
    Serial.print(" %");
    Serial.println("");
    JsonObject sht = doc.createNestedObject("sht40");
    sht["temperature"] = t2.temperature;
    sht["humidity"] = h2.relative_humidity;
  } else {
    Serial.println("SHT40 read error");
  }

  Serial.print("BMP 280:\tTemperature = ");
  Serial.print(bmp280.readTemperature());
  Serial.print(" °C");
  Serial.println("");
  JsonObject bmp = doc.createNestedObject("bmp280");
  bmp["temperature"] = bmp280.readTemperature();
  bmp["pressure"] = bmp280.readPressure() / 100.0;

  Serial.print("BH 1750:\tLight = ");
  Serial.print(bh1750.readLightLevel());
  Serial.print(" lux");
  Serial.println("");
  JsonObject bh = doc.createNestedObject("bh1750");
  bh["lux"] = bh1750.readLightLevel();

  tsl2561.getEvent(&l);
  if (l.light >= 0) {
    Serial.print("TSL 2561:\tLight = ");
    Serial.print(l.light);
    Serial.print(" lux");
    Serial.println("");
    JsonObject tsl = doc.createNestedObject("tsl2561");
    tsl["lux"] = l.light;
  } else {
    Serial.println("TSL 2561 read error");
  }
  Serial.println("");

  if (client.connected()) {
    mqtt_send(doc);
  }
  delay(2000);
}