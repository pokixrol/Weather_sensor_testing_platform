#include <Adafruit_Sensor.h>

#include <Wire.h>
#include "Adafruit_SHT4x.h"

TwoWire I2C_1 = TwoWire(0); // SHT45
TwoWire I2C_2 = TwoWire(1); // SHT40

Adafruit_SHT4x sht45 = Adafruit_SHT4x();
Adafruit_SHT4x sht40 = Adafruit_SHT4x();

void setup() {
  Serial.begin(115200);
  delay(1000);

  I2C_1.begin(21, 22);      // SDA, SCL
  I2C_1.setClock(100000);   
  if (!sht45.begin(&I2C_1)) Serial.println("SHT45 fail");  

  I2C_2.begin(18, 19);     
  I2C_2.setClock(100000);   
  if (!sht40.begin(&I2C_2)) Serial.println("SHT40 fail");  

  sht40.setPrecision(SHT4X_HIGH_PRECISION);
  sht45.setPrecision(SHT4X_HIGH_PRECISION);
}

void loop() {
  sensors_event_t h1, t1, h2, t2;

  sht40.getEvent(&h1, &t1);
  sht45.getEvent(&h2, &t2);

  Serial.print("SHT45: "); Serial.print(t1.temperature); Serial.print("°C, ");
  Serial.println(h1.relative_humidity);

  Serial.print("SHT40: "); Serial.print(t2.temperature); Serial.print("°C, ");
  Serial.println(h2.relative_humidity);

  delay(2000);
}
