#include <Adafruit_LTR390.h>
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
Adafruit_LTR390 ltr390 = Adafruit_LTR390();

float LTR390_Resolution(int resolution) {
  switch (resolution) {
    case LTR390_RESOLUTION_13BIT: return 3.125;
    case LTR390_RESOLUTION_16BIT: return 25;
    case LTR390_RESOLUTION_17BIT: return 50;
    case LTR390_RESOLUTION_18BIT: return 100;
    case LTR390_RESOLUTION_19BIT: return 200;
    case LTR390_RESOLUTION_20BIT: return 400;
  }
  return 25;
}

float LTR390_gain(int gain) {
  switch (gain) {
    case LTR390_GAIN_1: return 1;
    case LTR390_GAIN_3: return 3;
    case LTR390_GAIN_6: return 6;
    case LTR390_GAIN_9: return 9;
    case LTR390_GAIN_18: return 18;
  }
  return 1;
}

float calculate_ALS_Lux(int als) {
  float gain = LTR390_gain(ltr390.getGain());
  float resolution = LTR390_Resolution(ltr390.getResolution());
  float wfac = 1.0;
  return (0.6 * als) / (gain * (resolution / 100.0)) * wfac;
}

float calculate_UVS_Index(int uv) {
  float gain = LTR390_gain(ltr390.getGain());
  float resolution = LTR390_Resolution(ltr390.getResolution());
  float wfac = 1.0;
  float sensitivity = 2300.0 * (gain / 18.0) * (resolution / 400.0);
  return (uv / sensitivity) * wfac;
}

void check_sensor(bool result, const char* name) {
  if (!result) {
    Serial.print(name);
    Serial.println(" not found !");
  }
}

void setup() {
  Serial.begin(115200);

  Wire.begin();
  Wire1.begin(19, 18);

  while (!Serial) {}
  delay(200);

  check_sensor(bme688.begin(0x76), "BME 688");
  check_sensor(sht45.begin(&Wire), "SHT 45");
  check_sensor(bh1750.begin(), "BH 1750");
  check_sensor(tsl2561.begin(), "TSL 2561");
  check_sensor(sht40.begin(&Wire1), "SHT 40");
  check_sensor(bmp280.begin(0x77), "BMP 280");
  check_sensor(ltr390.begin(), "LTR 390");

  ltr390.setGain(LTR390_GAIN_3);
  ltr390.setResolution(LTR390_RESOLUTION_16BIT);

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
    Serial.print(" %\tPressure = ");
    Serial.print(bme688.pressure/100);
    Serial.print(" hPa");
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
  Serial.print(" °C\tPressure = ");
  Serial.print(bme688.readPressure()/100);
  Serial.print(" hPa");
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

  ltr390.setMode(LTR390_MODE_ALS);
  delay(200);
  ltr390.readALS();
  delay(50);
  int als = ltr390.readALS();
  Serial.print("LTR 390:\tLight = ");
  Serial.print(calculate_ALS_Lux(als),2);
  Serial.print(" lux");
  ltr390.setMode(LTR390_MODE_UVS);
  delay(200);
  ltr390.readUVS();
  delay(50);
  int uv = ltr390.readUVS();
  Serial.print(" \tUV Index = ");
  Serial.print(calculate_UVS_Index(uv),2);
  Serial.println("");
  JsonObject ltr = doc.createNestedObject("ltr390");
  ltr["lux"] = calculate_ALS_Lux(als);
  ltr["UVI"] = calculate_UVS_Index(uv);

  if (client.connected()) {
    mqtt_send(doc);
  }
  delay(2000);
}