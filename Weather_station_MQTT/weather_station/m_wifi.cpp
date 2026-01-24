#include <WiFi.h>
#include <WiFiManager.h>
#include "m_wifi.h"

void wifiConnection() {
  WiFi.mode(WIFI_STA);

  WiFiManager wm;

  wm.setConfigPortalTimeout(180);
  
  bool res = wm.autoConnect("ESP", "suprsecretpassword");
  if (!res) {
    Serial.println("Wifi failed to connect. Restart ...");
    ESP.restart();
  } else {
    Serial.println("Wifi connected");
  }
  randomSeed(micros());
}