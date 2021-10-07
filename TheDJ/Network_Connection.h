#ifndef NETWORK
#define NETWORK

#include <WiFiNINA.h>
#include "real_secrets.h"
//#include "arduino_secrets.h"

// initialize WiFi connection:
WiFiClient network_connection;


void connectToWiFi() {
  // initialize WiFi, if not connected:
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("Connecting to ");
    Serial.println(SECRET_SSID);
    WiFi.begin(SECRET_SSID, SECRET_PASS);
    delay(2000);
  }
  // print IP address once connected:
  Serial.print("Connected. My IP address: ");
  Serial.println(WiFi.localIP());
}
#endif
