#include <Adafruit_SHT4x.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_TSL2561_U.h>
#include <BH1750.h>
#include "m_wifi.h"
#include "m_thingspeak.h"
#include "secret.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"

Adafruit_BME680 bme688(&Wire);
BH1750 bh1750;
Adafruit_TSL2561_Unified tsl2561 = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 12345);
Adafruit_SHT4x sht45 = Adafruit_SHT4x();
Adafruit_SHT4x sht40 = Adafruit_SHT4x();
Adafruit_BMP280 bmp280(&Wire1);

void setup() {
  Serial.begin(115200);

  Wire.begin();
  Wire1.begin(19, 18);

  if (!bme688.begin(0x76)) {
    Serial.println("BME 688 not found !");
  } else {
    bme688.setTemperatureOversampling(BME680_OS_8X);
    bme688.setHumidityOversampling(BME680_OS_2X);
    bme688.setPressureOversampling(BME680_OS_4X);
    bme688.setIIRFilterSize(BME680_FILTER_SIZE_3);
    bme688.setGasHeater(320, 150);
  }

  if (!sht45.begin(&Wire)) {
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

  if (!sht40.begin(&Wire1)) {
    Serial.println("SHT40 not found !");
    delay(1000);
  }

  if (!bmp280.begin(0x77)) {
    Serial.println("BMP280 not found !");
    delay(1000);
  }

  wifiConnection();
}

void loop() {
  sensors_event_t l, h1, t1, h2, t2;

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

  if (sht45.getEvent(&h1, &t1)) {
    Serial.print("SHT 45:\t\tTemperature = ");
    Serial.print(t1.temperature);
    Serial.print(" °C\tHumidity = ");
    Serial.print(h2.relative_humidity);
    Serial.print(" %");
    Serial.println("");
  } else {
    Serial.println("SHT45 read error");
  }
  delay(1000);

  if (sht40.getEvent(&h2, &t2)) {
    Serial.print("SHT 40:\t\tTemperature = ");
    Serial.print(t2.temperature);
    Serial.print(" °C\tHumidity = ");
    Serial.print(h2.relative_humidity);
    Serial.print(" %");
    Serial.println("");
  } else {
    Serial.println("SHT40 read error");
  }
  delay(1000);

  Serial.print("BMP 280:\tTemperature = ");
  Serial.print(bmp280.readTemperature());
  Serial.print(" °C");
  Serial.println("");
  delay(1000);

  Serial.print("BH 1750:\tLight = ");
  Serial.print(bh1750.readLightLevel());
  Serial.print(" lux");
  Serial.println("");
  delay(1000);


  tsl2561.getEvent(&l);
  if (l.light >= 0) {
    Serial.print("TSL 2561:\tLight = ");
    Serial.print(l.light);
    Serial.print(" lux");
    Serial.println("");
  } else {
    Serial.println("TSL 2561 read error");
  }
  delay(1000);

  postData(bme688.temperature, t1.temperature, bme688.humidity, h1.relative_humidity, bh1750.readLightLevel(), l.light, bmp280.readTemperature());
  delay(15000);
}