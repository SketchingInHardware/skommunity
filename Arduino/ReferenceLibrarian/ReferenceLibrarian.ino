/*
2021 October 03
Message Seeder Code v 0.0.1
*/

#include "MQTT_Handler.h"
#include "Network_Connection.h"

//Message Generators
#include "Generators.h"


//----------------------------------------------------   SETUP
void setup() {

  // initialize serial:
  Serial.begin(9600);
  // wait for serial monitor to open:
  while (!Serial);

  connectToWiFi();
  connectToMQTT();
  
}


//----------------------------------------------------   LOOP
void loop() {

//  //-------------------------------------  Read Binary Hardware
//
//  //-------------------------------------  Read Analog Hardware
//
//  //-------------------------------------  Update Local World

  //-------------------------------------  Update Remote World
  // if not connected to the broker, try to connect:
  if (!mqttClient.connected()) {
    Serial.println("reconnecting");
    connectToBroker();
  }


    sendMQTTTimedObject(&scaleToneMessageObject);
    sendMQTTTimedObject(&ellapsedMillisObject);

    sendMQTTTimedObject(&randomMessageObject);

    //will be at least 3 seconds
    sendMQTTConditionalMessage(&irregularRandom);


}

//----------------------------------------------------   END LOOP
