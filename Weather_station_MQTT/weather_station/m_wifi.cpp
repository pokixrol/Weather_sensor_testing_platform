// Libraries for Wi-Fi functionality
#include <WiFi.h>
#include <WiFiManager.h>
#include "m_wifi.h"

void wifi_connection() {
  // Set Wi-Fi mode to station
  WiFi.mode(WIFI_STA);

  // Create WiFiManager object
  WiFiManager wm;

  // Set timeout for configuration portal
  wm.setConfigPortalTimeout(180);  //seconds

  // Try to connect to saved Wi-Fi credentials
  // If not available, create an AP with given name and password
  bool res = wm.autoConnect("ESP", "suprsecretpassword");
  // If connection fails, restart the device
  if (!res) {
    Serial.println("Wifi failed to connect. Restart ...");
    ESP.restart();
  } else {
    Serial.println("Wifi connected");
  }
}