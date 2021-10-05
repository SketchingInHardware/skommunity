/*
2021 October 03
Message Seeder Code v 0.0.1
*/

#include "MQTT_Handler.h"
#include "Network_Connection.h"

#include "Personality.h"


//----------------------------------------------------   SETUP
void setup() {

  // initialize serial:
  Serial.begin(9600);
  // wait for serial monitor to open:
  while (!Serial);

  connectToWiFi();
  connectToMQTT();

  setMusings();
}


//----------------------------------------------------   LOOP
void loop() {

  
    muse();

 
}

//----------------------------------------------------   END LOOP
