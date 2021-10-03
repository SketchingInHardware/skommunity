/*
2021 October 03
Message Seeder Code v 0.0.1
*/

#include "MQTT_ConnectionHandler.h"
#include "Network_Connection.h"
#include "HardwareHandler.h"

//Message Generators
#include "Poem.h"
#include "RandomNumber.h"
#include "Scales.h"


//----------------------------------------------------   SETUP
void setup() {
  // pin settings
  pinMode(pushButton, INPUT);
  for (int thisPin = cillium4; thisPin <= cillium1; thisPin++) {
    pinMode(thisPin, OUTPUT);
  }

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
//  checkButton();
//
//  //-------------------------------------  Read Analog Hardware
//  updateFolicleData();
//
//  //-------------------------------------  Update Local World
//
//  updateCiliaData();
//
//  if (buttonState == true) {
//    printFolicleData();
//    Serial.println("------");
//    printFolicleAverage();
//    Serial.println(" ");
//    printCiliaData();
//    Serial.println(" ");
//  }
//
//  updateCilia();


  //-------------------------------------  Update Remote World
  // if not connected to the broker, try to connect:
  if (!mqttClient.connected()) {
    Serial.println("reconnecting");
    connectToBroker();
  }

    sendMQTTObject(&randomMessageObject);
    sendMQTTObject(&scaleToneMessageObject);
  
//    updatePoemLine();
//    updatePoemLineInterval(&poemMessageObject);
//    sendMQTTObject(&poemMessageObject);

//  sendMQTTObject(&hungerMessageObject);
//  sendMQTTObject(&strokeMessageObject);

}

//----------------------------------------------------   END LOOP
