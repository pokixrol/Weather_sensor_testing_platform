#include <Adafruit_TSL2561_U.h>

#include <BH1750.h>

#include <WiFiManager.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"
#include <ArtronShop_SHT45.h>

Adafruit_BME680 bme688(&Wire);
ArtronShop_SHT45 sht45(&Wire, 0x44);
BH1750 bh1750;
Adafruit_TSL2561_Unified tsl2561 = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 12345);

String serverName = "api.thingspeak.com";
String apiKey = "1M4TIMF8LJI5ASDY";

void wifiConnection() {
  WiFi.mode(WIFI_STA);
  WiFiManager wm;
  wm.setConfigPortalTimeout(180);
  bool res = wm.autoConnect("ESP", "suprtajneheslo");
  if (!res) {
    Serial.println("Wifi se nepodařilo připojit. Restart ...");
    ESP.restart();
  } else {
    Serial.println("Wifi připojeno");
  }
}

void postData(float tbme688, float tsht45, float hbme688, float hsht45, float bh1750, float tsl2561) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String serverPath = "http://" + serverName + "/update?api_key=1M4TIMF8LJI5ASDY&field1=" + tbme688
                        + "&field2=" + tsht45
                        + "&field3=" + hbme688
                        + "&field4=" + hsht45
                        + "&field5=" + bh1750
                        + "&field6=" + tsl2561;

    http.begin(serverPath.c_str());
    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
      Serial.print("HTTP response:   ");
      Serial.println(httpResponseCode);
      Serial.println(http.getString());
    } else {
      Serial.print("Error code:   ");
      Serial.println(httpResponseCode);
    }
    http.end();
  } else {
    Serial.println("WiFi odpojeno");
  }
}

void setup() {
  Serial.begin(115200);
  Wire.begin();

  if (!bme688.begin()) {
    Serial.println("BME 688 not found !");
  } else {
    bme688.setTemperatureOversampling(BME680_OS_8X);
    bme688.setHumidityOversampling(BME680_OS_2X);
    bme688.setPressureOversampling(BME680_OS_4X);
    bme688.setIIRFilterSize(BME680_FILTER_SIZE_3);
    bme688.setGasHeater(320, 150);
  }

  if (!sht45.begin()) {
    Serial.println("SHT 45 not found !");
    delay(1000);
  }

  if (!bh1750.begin()) {
    Serial.println("BH 1750 not found !");
    delay(1000);
  }

  if (!tsl2561.begin()) {
    Serial.println("TSL 2561 not found !");
    delay(1000);
  }

  wifiConnection();
}

void loop() {
  sensors_event_t tsl_event;
  
  if (bme688.performReading()) {

    Serial.print("BME 688:\tTemperature = ");
    Serial.print(bme688.temperature);
    Serial.print(" °C\tHumidity = ");
    Serial.print(bme688.humidity);
    Serial.print(" %");
    Serial.println("");
  } else {
    Serial.println("BME 688 read error");
  }
  delay(1000);

  if (sht45.measure()) {
    Serial.print("SHT 45:\t\tTemperature = ");
    Serial.print(sht45.temperature(), 1);
    Serial.print(" °C\tHumidity = ");
    Serial.print(sht45.humidity(), 1);
    Serial.print(" %");
    Serial.println("");
  } else {
    Serial.println("SHT45 read error");
  }
  delay(1000);

  Serial.print("BH 1750:\tLight = ");
  Serial.print(bh1750.readLightLevel());
  Serial.print(" lux");
  Serial.println("");
  delay(1000);


  tsl2561.getEvent(&tsl_event);
  if (tsl_event.light) {
    Serial.print("TSL 2561:\tLight = ");
    Serial.print(tsl_event.light);
    Serial.print(" lux");
    Serial.println("");
  } else {
    Serial.println("TSL 2561 read error");
  }
  delay(1000);

  postData(bme688.temperature, sht45.temperature(), bme688.humidity, sht45.humidity(), bh1750.readLightLevel(), tsl_event.light);
  delay(15000);
}