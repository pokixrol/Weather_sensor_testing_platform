#include <WiFi.h>
#include <WiFiManager.h>
#include "m_wifi.h"

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