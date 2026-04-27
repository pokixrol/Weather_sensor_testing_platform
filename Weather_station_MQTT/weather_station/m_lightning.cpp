#include "m_lightning.h"
#include <SparkFun_AS3935.h>
#include <Wire.h>

#define AS3935_ADDR 0x03  // AS3935 I2C address

// Operating modes
#define INDOOR 0x12
#define OUTDOOR 0x0E

// Interrupt source flags
#define LIGHTNING_INT 0x08
#define DISTURBER_INT 0x04
#define NOISE_INT 0x01

// Lightning sensor object
SparkFun_AS3935 as3935(AS3935_ADDR);

// GPIO pin connected to lightning interrupt output
const int lightningPin = 4;
// Flag set by interrupt when lightning is detected
volatile bool lightningFlag = false;

// Interrupt Service Routine (ISR)
void IRAM_ATTR lightningISR() {
  lightningFlag = true;
}

void lightning_init() {

  as3935.begin();

  // Configure sensor indoor or outdoor sensitivity settings

  // as3935.setIndoorOutdoor(OUTDOOR);
  // as3935.setNoiseLevel(3);         0–7
  // as3935.watchdogThreshold(2);     0–10
  // as3935.setSpikeRejection(3);     0–11
  // as3935.maskDisturber(false);

  as3935.setIndoorOutdoor(INDOOR);
  as3935.setNoiseLevel(4);
  as3935.watchdogThreshold(3);
  as3935.spikeRejection(5);
  as3935.maskDisturber(true);

  // Configure interrupt pin
  pinMode(lightningPin, INPUT);
  // Configure interrupt pin
  attachInterrupt(digitalPinToInterrupt(lightningPin), lightningISR, RISING);
}

// Check if lightning event was detected
bool lightning_available() {
  return lightningFlag;
}

// Read lightning data and store it into JSON
void lightning_read(JsonDocument &doc) {

  // Reset flag after reading
  lightningFlag = false;

  int source = as3935.readInterruptReg();

  if (source == LIGHTNING_INT) {

    doc["type"] = "lightning";
    doc["distance_km"] = as3935.distanceToStorm();
    doc["energy"] = as3935.lightningEnergy();
  }
}