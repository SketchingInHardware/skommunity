/*
2021 October 03
Message Seeder Code v 0.0.1
*/

#include "MQTT_Handler.h"
#include "Network_Connection.h"

#include "Personality.h"




//----------------------------------------------------   SETUP
void setup() {


  //Serial for network debugging
  // initialize serial:
  Serial.begin(9600);

  connectToWiFi();
  connectToMQTT();

}


//----------------------------------------------------   LOOP
void loop() {

  worry();

}

//----------------------------------------------------   END LOOP
