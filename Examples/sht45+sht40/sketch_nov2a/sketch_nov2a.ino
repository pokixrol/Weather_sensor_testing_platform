#include <Adafruit_Sensor.h>
#include <Wire.h>
#include "Adafruit_SHT4x.h"

Adafruit_SHT4x sht45 = Adafruit_SHT4x();
Adafruit_SHT4x sht40 = Adafruit_SHT4x();

void setup() {
  Serial.begin(115200);

  Wire.begin();
  Wire1.begin( 19, 18);     

  sht40.begin(&Wire1);
  sht45.begin(&Wire);

  sht40.setPrecision(SHT4X_HIGH_PRECISION);
  sht45.setPrecision(SHT4X_HIGH_PRECISION);
}

void loop() {
  sensors_event_t h1, t1, h2, t2;

  sht40.getEvent(&h1, &t1);
  sht45.getEvent(&h2, &t2);

  Serial.print("SHT45: "); Serial.print(t1.temperature); Serial.print("°C, ");
  Serial.println(h1.relative_humidity);
delay(2000);
  Serial.print("SHT40: "); Serial.print(t2.temperature); Serial.print("°C, ");
  Serial.println(h2.relative_humidity);

  delay(2000);
}