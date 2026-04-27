#include <Wire.h>
#include <Adafruit_SHT4x.h>
#include <Adafruit_BMP280.h>

Adafruit_SHT4x sht4 = Adafruit_SHT4x();
Adafruit_BMP280 bmp; 

void setup() {
  Serial.begin(115200);
  delay(2000);

  Wire.begin(21, 22); 


  if (!sht4.begin()) {
    Serial.println("Nepodarilo se najit SHT4x ");
  } else {
    Serial.println("SHT4x senzor detekovan.");
    sht4.setPrecision(SHT4X_HIGH_PRECISION);
    sht4.setHeater(SHT4X_NO_HEATER);
  }

  if (!bmp.begin(0x76) && !bmp.begin(0x77)) {
    Serial.println("Nepodarilo se najit BMP280");
  } else {
    Serial.println("BMP280 senzor detekovan.");
  }
}

void loop() {

  sensors_event_t humidity, temp;
  if (sht4.getEvent(&humidity, &temp)) {
    Serial.print("Teplota (SHT40): ");
    Serial.print(temp.temperature);
    Serial.println(" °C");

    Serial.print("Vlhkost: ");
    Serial.print(humidity.relative_humidity);
    Serial.println(" %");
  }

  Serial.print("Teplota (BMP280): ");
  Serial.print(bmp.readTemperature());
  Serial.println(" °C");

  Serial.print("Tlak: ");
  Serial.print(bmp.readPressure() / 100.0F);
  Serial.println(" hPa");

  Serial.println("-----------------------------\n");
  delay(2000);
}
