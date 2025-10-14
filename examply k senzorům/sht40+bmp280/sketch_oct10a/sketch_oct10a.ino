#include <Wire.h>
#include <Adafruit_SHT4x.h>
#include <Adafruit_BMP280.h>

// --- Vytvoření objektů senzorů ---
Adafruit_SHT4x sht4 = Adafruit_SHT4x();
Adafruit_BMP280 bmp; // BMP280 senzor

void setup() {
  Serial.begin(115200);
  delay(2000);
  Serial.println("Zacinam inicializaci senzoru...");

  Wire.begin(21, 22);  // ESP32 standardní piny SDA=21, SCL=22

  // --- Inicializace SHT40 ---
  if (!sht4.begin()) {
    Serial.println("❌ Nepodarilo se najit SHT4x senzor!");
  } else {
    Serial.println("✅ SHT4x senzor detekovan.");
    sht4.setPrecision(SHT4X_HIGH_PRECISION);
    sht4.setHeater(SHT4X_NO_HEATER);
  }

  // --- Inicializace BMP280 ---
  if (!bmp.begin(0x76) && !bmp.begin(0x77)) {
    Serial.println("❌ Nepodarilo se najit BMP280 senzor!");
  } else {
    Serial.println("✅ BMP280 senzor detekovan.");
  }

  Serial.println("Inicializace dokoncena.\n");
}

void loop() {
  // --- Čtení SHT40 ---
  sensors_event_t humidity, temp;
  if (sht4.getEvent(&humidity, &temp)) {
    Serial.print("🌡️  Teplota (SHT40): ");
    Serial.print(temp.temperature);
    Serial.println(" °C");

    Serial.print("💧 Vlhkost: ");
    Serial.print(humidity.relative_humidity);
    Serial.println(" %");
  }

  // --- Čtení BMP280 ---
  Serial.print("🌡️  Teplota (BMP280): ");
  Serial.print(bmp.readTemperature());
  Serial.println(" °C");

  Serial.print("🌬️  Tlak: ");
  Serial.print(bmp.readPressure() / 100.0F);
  Serial.println(" hPa");

  Serial.println("-----------------------------\n");
  delay(2000);
}
