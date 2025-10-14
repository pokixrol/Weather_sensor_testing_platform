#include <WiFiManager.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"
#include <ArtronShop_SHT45.h>

Adafruit_BME680 bme(&Wire);
ArtronShop_SHT45 sht45(&Wire, 0x44);

String serverName = "api.thingspeak.com";
String apiKey = "1M4TIMF8LJI5ASDY";

void wifiConnection() {
  WiFi.mode(WIFI_STA);
  WiFiManager wm;
  wm.setConfigPortalTimeout(180);
  bool res;
  res = wm.autoConnect("ESP", "suprtajneheslo");
  if (!res) {
    Serial.println("Wifi se nepodařilo připojit");
    ESP.restart();
  } else {
    Serial.println("Wifi připojeno");
  }
}

void postData(float tbme, float tsht, float hbme, float hsht) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String serverPath = "http://" + serverName + "/update?api_key=1M4TIMF8LJI5ASDY&field1=" + tbme
                        + "&field2=" + tsht
                        + "&field3=" + hbme
                        + "&field4=" + hsht;
    http.begin(serverPath.c_str());
    int httpResponseCode = http.GET();
    if (httpResponseCode > 0) {
      Serial.print("HTTP response:   ");
      Serial.println(httpResponseCode);
      String payload = http.getString();
      Serial.println(payload);
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

  if (!bme.begin()) {
    Serial.println("Could not find a valid BME680 sensor, check wiring!");
    while (1)
      ;
  }

  // Set up oversampling and filter initialization
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150);

  Wire.begin();
  while (!sht45.begin()) {
    Serial.println("SHT45 not found !");
    delay(1000);
  }
  wifiConnection();
  sht45.measure();
  postData(bme.readTemperature(), sht45.temperature(), bme.readHumidity(), sht45.humidity());
}

void loop() {
  if (!bme.performReading()) {
    Serial.println("Failed to perform reading :(");
    return;
  }
  Serial.println("BME 688:");
  Serial.print("Temperature = ");
  Serial.print(bme.temperature);
  Serial.print(" *C\tHumidity = ");
  Serial.print(bme.humidity);
  Serial.print(" %");
  Serial.println();
  delay(5000);

  if (sht45.measure()) {
    Serial.println("SHT45:");
    Serial.print("Temperature = ");
    Serial.print(sht45.temperature(), 1);
    Serial.print(" *C\tHumidity = ");
    Serial.print(sht45.humidity(), 1);
    Serial.print(" %RH");
    Serial.println();
  } else {
    Serial.println("SHT45 read error");
  }
  delay(5000);
  postData(bme.temperature, sht45.temperature(), bme.humidity, sht45.humidity());
  delay(10000);
}
