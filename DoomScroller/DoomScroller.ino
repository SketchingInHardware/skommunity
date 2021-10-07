/*
2021 October 03
Message Seeder Code v 0.0.1
*/

#include "MQTT_Handler.h"
#include "Network_Connection.h"
#include "HardwareHandler.h"

//Message Generators
#include "SenderExamples.h"

#include "Personality.h"

//----------------------------------------------------   SETUP
void setup() {
  // pin settings
  hardwareSetUp();

  // initialize serial:
  Serial.begin(9600);

  connectToWiFi();
  connectToMQTT();

  chooseFeed();
  setExhaustionTable();
}


//----------------------------------------------------   LOOP
void loop() {

haveFeelings(millis());
doomScroll();

}

//----------------------------------------------------   END LOOP
