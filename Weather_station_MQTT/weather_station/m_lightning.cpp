#include "m_lightning.h"
#include <SparkFun_AS3935.h>
#include <Wire.h>

#define AS3935_ADDR 0x03
#define INDOOR 0x12
#define OUTDOOR 0x0E
#define LIGHTNING_INT 0x08
#define DISTURBER_INT 0x04
#define NOISE_INT 0x01

SparkFun_AS3935 as3935(AS3935_ADDR);

const int lightningPin = 4;
volatile bool lightningFlag = false;

void IRAM_ATTR lightningISR() {
  lightningFlag = true;
}

void lightning_init() {

  as3935.begin();

  // as3935.setIndoorOutdoor(OUTDOOR);
  // as3935.setNoiseLevel(2);         0–7
  // as3935.watchdogThreshold(2);     0–10
  // as3935.setSpikeRejection(2);     0–11
  // as3935.maskDisturber(false);

  as3935.setIndoorOutdoor(INDOOR);
  as3935.setNoiseLevel(4);
  as3935.watchdogThreshold(3);
  as3935.spikeRejection(5);
  as3935.maskDisturber(true);

  pinMode(lightningPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(lightningPin), lightningISR, RISING);
}

bool lightning_available() {
  return lightningFlag;
}

void lightning_read(JsonDocument &doc) {

  lightningFlag = false;

  int source = as3935.readInterruptReg();

  if (source == LIGHTNING_INT) {

    doc["type"] = "lightning";
    doc["distance_km"] = as3935.distanceToStorm();
    doc["energy"] = as3935.lightningEnergy();
  }
}