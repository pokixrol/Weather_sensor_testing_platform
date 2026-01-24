#include <WiFi.h>
#include <HTTPClient.h>
#include "m_thingspeak.h"
#include "secret.h"

String serverName = "api.thingspeak.com";

void postData(float tbme688, float tsht45, float hbme688, float hsht45, float lbh1750, float ltsl2561, float tbmp) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String serverPath = "http://" + serverName + "/update?api_key=" API_KEY "&field1=" + tbme688
                        + "&field2=" + tsht45
                        + "&field3=" + hbme688
                        + "&field4=" + hsht45
                        + "&field5=" + lbh1750
                        + "&field6=" + ltsl2561
                        + "&field7=" + tbmp;

    http.begin(serverPath.c_str());
    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
      Serial.print("HTTP response:   ");
      Serial.println(httpResponseCode);
      Serial.println(http.getString());
    } else {
      Serial.print("Error code:   ");
      Serial.println(httpResponseCode);
    }
    http.end();
  } else {
    Serial.println("WiFi odpojeno");
  }
}